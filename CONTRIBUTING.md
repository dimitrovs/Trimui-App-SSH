# Contributing to Dropbear SSH for TrimUI Smart Pro

Thank you for your interest in contributing! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Project Structure](#project-structure)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

## Code of Conduct

- Be respectful and constructive in all interactions
- Focus on what is best for the community
- Show empathy towards other community members
- Accept constructive criticism gracefully

## Getting Started

### Prerequisites

- Cross-compilation toolchain: `aarch64-linux-gnu-g++`
- SDL2 development libraries
- SDL2_ttf development libraries
- Basic knowledge of C++11/14
- Familiarity with Linux system programming

### First Contribution

Good first issues include:
- Documentation improvements
- Adding more tests
- Fixing compiler warnings
- Code comments and clarity improvements
- Bug fixes with clear reproduction steps

## Development Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/Maxwell-SS/trimui-smart-pro-build-system.git
   cd trimui-smart-pro-build-system/apps/Trimui-App-SSH
   ```

2. **Build the project**
   ```bash
   make clean
   make
   ```

3. **Run tests**
   ```bash
   make test
   ```

4. **Clean build artifacts**
   ```bash
   make clean
   ```

## Project Structure

```
src/
├── main.cpp              # Application entry point
├── Application.h/cpp     # Main application orchestrator
├── DropbearManager.h/cpp # SSH server lifecycle management
├── NetworkManager.h/cpp  # Network interface discovery
├── Renderer.h/cpp        # SDL rendering logic
├── PathHelper.h/cpp      # Path resolution utilities
├── Color.h               # Color definitions
└── Constants.h           # Application constants

tests/
├── test_main.cpp         # Test framework and runner
├── test_PathHelper.cpp   # Path resolution tests
├── test_NetworkManager.cpp # Network tests
└── test_Color.cpp        # Color utilities tests
```

### Key Components

- **Application**: SDL lifecycle, event handling, component coordination
- **DropbearManager**: Process management, host key generation, log streaming
- **NetworkManager**: Network interface enumeration, IPv4 discovery
- **Renderer**: All SDL2 rendering operations
- **PathHelper**: Executable and resource path resolution

## Coding Standards

### C++ Style

- **Standard**: C++11/14
- **Indentation**: 4 spaces (no tabs)
- **Line length**: Max 100 characters preferred
- **Naming conventions**:
  - Classes: `PascalCase`
  - Functions/methods: `camelCase`
  - Variables: `snake_case` or `camelCase`
  - Constants: `UPPER_CASE` or namespaced constants
  - Private members: trailing underscore `member_`

### Code Organization

- One class per header/implementation file pair
- Header guards: Use `#pragma once`
- Include order: System headers, SDL headers, project headers
- Forward declarations where possible
- RAII for all resource management

### Best Practices

```cpp
// Good: RAII, const-correctness, clear naming
class ResourceManager {
public:
    explicit ResourceManager(const std::string& path);
    ~ResourceManager();
    
    // Delete copy operations
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    bool initialize();
    std::string getPath() const;
    
private:
    std::string path_;
    SDL_Texture* texture_ = nullptr;
};

// Good: Use of const, references, modern C++
void processData(const std::vector<std::string>& items) {
    for (const auto& item : items) {
        // Process item
    }
}

// Good: Smart pointers for ownership
std::unique_ptr<Manager> manager_ = std::make_unique<Manager>();
```

### What to Avoid

```cpp
// Bad: Manual memory management
Manager* manager = new Manager();
delete manager; // Prefer RAII/smart pointers

// Bad: Magic numbers
if (count > 300) { /* ... */ }
// Good: Named constants
if (count > LogDisplay::MAX_LINES) { /* ... */ }

// Bad: Unclear ownership
SDL_Texture* getTexture(); // Who owns this?
// Good: Clear ownership
std::shared_ptr<SDL_Texture> getTexture();
```

## Testing

### Writing Tests

All new features and bug fixes should include tests.

```cpp
void registerMyFeatureTests(TestRunner& runner) {
    runner.addTest("Feature does something correctly", []() {
        MyClass obj;
        ASSERT_TRUE(obj.doSomething());
    });
    
    runner.addTest("Feature handles errors", []() {
        MyClass obj;
        ASSERT_FALSE(obj.doSomethingInvalid());
    });
}
```

### Test Coverage

- Unit tests for all utility functions
- Integration tests for component interactions
- Edge case testing (null pointers, empty strings, boundary values)
- Error handling verification

### Running Tests

```bash
# Build and run all tests
make test

# Run specific test binary
./build/tests/test_runner
```

## Pull Request Process

### Before Submitting

1. **Run tests**: Ensure all tests pass
   ```bash
   make clean
   make test
   ```

2. **Code style**: Follow the coding standards above

3. **Documentation**: Update README.md if adding features

4. **Commits**: Write clear, descriptive commit messages
   ```
   Add feature to display IPv6 addresses
   
   - Extend NetworkManager to support IPv6
   - Add IPv6 formatting tests
   - Update UI to display both IPv4 and IPv6
   ```

### Submission

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Commit your changes with clear messages
7. Push to your fork
8. Open a Pull Request

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Performance improvement
- [ ] Code refactoring

## Testing
- [ ] All existing tests pass
- [ ] Added new tests for changes
- [ ] Tested on actual hardware (if applicable)

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex logic
- [ ] Documentation updated
- [ ] No new compiler warnings
```

### Review Process

- Maintainers will review within 1-2 weeks
- Address review feedback promptly
- Be open to suggestions and improvements
- Squash commits if requested before merge

## Reporting Issues

### Bug Reports

Include:
- **Description**: Clear description of the bug
- **Steps to Reproduce**: Numbered steps
- **Expected Behavior**: What should happen
- **Actual Behavior**: What actually happens
- **Environment**: Device, OS version, build info
- **Logs**: Relevant log output if available

### Feature Requests

Include:
- **Use Case**: Why is this feature needed?
- **Proposed Solution**: How should it work?
- **Alternatives**: Other approaches considered
- **Additional Context**: Screenshots, examples, etc.

### Issue Template

```markdown
**Description**
Clear and concise description

**Steps to Reproduce**
1. Launch application
2. Navigate to...
3. Press...

**Expected Behavior**
Should display...

**Actual Behavior**
Instead shows...

**Environment**
- Device: TrimUI Smart Pro
- Build: [commit hash or version]

**Logs**
```
Paste relevant logs here
```
```

## Questions?

- Check existing issues and pull requests first
- Ask in issue comments
- Be patient and respectful

Thank you for contributing! 🎮
