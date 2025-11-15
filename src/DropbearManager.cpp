#include "DropbearManager.h"
#include "PathHelper.h"
#include "Constants.h"
#include <SDL2/SDL.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cerrno>
#include <cstring>
#include <cstdio>

DropbearManager::DropbearManager(LogCallback logCallback)
    : log_callback_(std::move(logCallback)) {
}

DropbearManager::~DropbearManager() {
    stop();
}

bool DropbearManager::start() {
    const std::string db_path = PathHelper::bundledDropbearPath();
    
    if (!isExecutable(db_path)) {
        log_callback_("dropbear not found or not executable at: " + db_path);
        return false;
    }
    
    log_callback_("starting bundled dropbear at: " + db_path);

    // Make sure we have a host key next to the binary
    if (!ensureHostKey()) {
        log_callback_("WARNING: could not create host key, Dropbear may fail.");
    }

    int pipefd[2];
    if (!createLogPipe(pipefd)) {
        return false;
    }

    dropbear_pid_ = fork();
    if (dropbear_pid_ == -1) {
        log_callback_(std::string("fork failed: ") + strerror(errno));
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (dropbear_pid_ == 0) {
        executeDropbear(pipefd, db_path);
    } else {
        // Parent keeps the read end for streaming logs
        close(pipefd[1]);
        dropbear_fd_ = pipefd[0];
        return true;
    }
    
    return false; // Should never reach here
}

void DropbearManager::stop() {
    if (dropbear_fd_ >= 0) {
        close(dropbear_fd_);
        dropbear_fd_ = -1;
    }
    
    if (dropbear_pid_ > 0) {
        stopDropbearGracefully();
        dropbear_pid_ = -1;
    }
}

void DropbearManager::stopDropbearGracefully() {
    kill(dropbear_pid_, SIGTERM);
    
    // Give it a moment to exit cleanly
    for (int i = 0; i < Dropbear::MAX_WAIT_ATTEMPTS; ++i) {
        int status;
        if (waitpid(dropbear_pid_, &status, WNOHANG) > 0) {
            return; // Process exited cleanly
        }
        SDL_Delay(Dropbear::WAIT_DELAY_MS);
    }
    
    // Force kill if still running
    kill(dropbear_pid_, SIGKILL);
    int status;
    waitpid(dropbear_pid_, &status, 0);
}

void DropbearManager::pumpLogs() {
    if (dropbear_fd_ < 0) return;

    char buf[1024];
    for (;;) {
        ssize_t n = read(dropbear_fd_, buf, sizeof(buf));
        if (n > 0) {
            pending_chunk_.append(buf, buf + n);
            flushPendingLines(false);
        } else if (n == 0) {
            // EOF from child; flush remainder and stop
            flushPendingLines(true);
            close(dropbear_fd_);
            dropbear_fd_ = -1;
            break;
        } else if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // No more data right now
            break;
        } else if (n == -1 && errno == EINTR) {
            continue;
        } else {
            // Unexpected read error
            log_callback_(std::string("read error: ") + strerror(errno));
            break;
        }
    }
}

bool DropbearManager::ensureHostKey() {
    const std::string keyPath = PathHelper::hostKeyPath();

    // Already present?
    if (fileExists(keyPath)) {
        log_callback_("Using existing host key: " + keyPath);
        return true;
    }

    return generateHostKey(keyPath);
}

bool DropbearManager::fileExists(const std::string& path) const {
    struct stat st{};
    return stat(path.c_str(), &st) == 0;
}

bool DropbearManager::isExecutable(const std::string& path) const {
    struct stat st{};
    return stat(path.c_str(), &st) == 0 && (st.st_mode & S_IXUSR);
}

bool DropbearManager::generateHostKey(const std::string& keyPath) {
    const std::string keygenPath = PathHelper::bundledDropbearKeygenPath();
    
    if (!isExecutable(keygenPath)) {
        log_callback_("dropbearkey not found or not executable at: " + keygenPath);
        return false;
    }

    log_callback_("Generating RSA host key (first run may take a while)...");
    
    pid_t pid = fork();
    if (pid < 0) {
        log_callback_(std::string("fork for dropbearkey failed: ") + strerror(errno));
        return false;
    }

    if (pid == 0) {
        executeDropbearKeygen(keygenPath, keyPath);
    }

    return waitForKeygenCompletion(pid, keyPath);
}

[[noreturn]] void DropbearManager::executeDropbearKeygen(const std::string& keygenPath, 
                                                          const std::string& keyPath) {
    execl(keygenPath.c_str(),
          "dropbearkey", "-t", "rsa", "-f", keyPath.c_str(),
          (char*)nullptr);
    
    int err = errno;
    dprintf(STDERR_FILENO, "exec %s failed: %s\n",
            keygenPath.c_str(), strerror(err));
    _exit(127);
}

bool DropbearManager::waitForKeygenCompletion(pid_t pid, const std::string& keyPath) {
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        log_callback_(std::string("waitpid(dropbearkey) failed: ") + strerror(errno));
        return false;
    }
    
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        log_callback_("dropbearkey exited with error");
        return false;
    }

    log_callback_("Host key generated at: " + keyPath);
    return true;
}

bool DropbearManager::createLogPipe(int pipefd[2]) {
    if (pipe(pipefd) == -1) {
        log_callback_(std::string("pipe failed: ") + strerror(errno));
        return false;
    }

    if (fcntl(pipefd[0], F_SETFL, O_NONBLOCK) == -1) {
        log_callback_(std::string("fcntl(O_NONBLOCK) failed: ") + strerror(errno));
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    return true;
}

[[noreturn]] void DropbearManager::executeDropbear(int pipefd[2], const std::string& db_path) {
    // Child: connect stdout/stderr to pipe's write end
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    close(pipefd[1]);

    const std::string keyPath = PathHelper::hostKeyPath();
    
    // Run Dropbear in foreground, with explicit host key
    execl(db_path.c_str(),
          "dropbear",
          "-E",         // log to stderr
          "-F",         // no daemonize
          "-r", keyPath.c_str(),
          (char*)nullptr);

    int err = errno;
    dprintf(STDERR_FILENO, "exec %s failed: %s\n",
            db_path.c_str(), strerror(err));
    _exit(127);
}

void DropbearManager::trimCR(std::string& s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();
}

void DropbearManager::flushPendingLines(bool force) {
    size_t start = 0;
    for (;;) {
        size_t pos = pending_chunk_.find('\n', start);
        if (pos == std::string::npos) break;
        std::string line = pending_chunk_.substr(start, pos - start);
        trimCR(line);
        if (!line.empty()) log_callback_(line);
        start = pos + 1;
    }
    if (force) {
        std::string tail = pending_chunk_.substr(start);
        trimCR(tail);
        if (!tail.empty()) log_callback_(tail);
        pending_chunk_.clear();
    } else if (start > 0) {
        pending_chunk_.erase(0, start); // keep incomplete line
    }
}
