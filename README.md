# ATM Management System - Improved Version

A comprehensive ATM management system written in C with enhanced error handling, improved user interface, and better code structure.

## 🚀 Features

### Core Banking Operations
- **User Authentication**: Secure login/registration with encrypted passwords
- **Account Management**: Create, update, view, and delete bank accounts
- **Multiple Account Types**: 
  - Savings (7% annual interest, monthly payout)
  - Current (no interest)
  - Fixed deposits (1, 2, 3 years with 4%, 5%, 8% interest respectively)
- **Transactions**: Deposit and withdraw money with validation
- **Account Transfer**: Transfer ownership between users
- **Balance Inquiry**: Quick balance checks
- **Account Listing**: View all owned accounts

### Enhanced Features
- **Colorful UI**: ANSI color-coded interface for better user experience
- **Robust Error Handling**: Comprehensive error codes and messages
- **Input Validation**: Strong validation for all user inputs
- **Data Integrity**: File-based storage with validation checks
- **Logging**: Error logging for debugging and audit trails

## 🏗️ Architecture Improvements

### Code Structure
```
src/
├── header.h          # Main header with all declarations
├── main.c           # Main program flow and menu handling
├── auth.c           # Authentication and user management
├── system.c         # Core banking operations
├── secondary.c      # Utility functions and validation
├── ui.c             # User interface and display functions
└── error_handler.c  # Error handling and logging
```

### Key Improvements Made

#### 1. **Error Handling**
- Replaced `goto` statements with proper error codes
- Comprehensive error enumeration with descriptive messages
- Centralized error handling and logging
- Graceful error recovery with user-friendly options

#### 2. **User Interface**
- Colorful, professional-looking interface
- Clear menu structures with visual separators
- Consistent input prompts with validation feedback
- Success/error message formatting with icons

#### 3. **Code Quality**
- Modular design with separated concerns
- Consistent naming conventions
- Proper memory management
- Input buffer handling
- Compiler warning fixes

#### 4. **Input Validation**
- Robust numeric validation
- String content validation
- Date format validation
- Amount range checking
- Account number format validation

## 🛠️ Building and Running

### Prerequisites
- GCC compiler
- Make utility
- POSIX-compliant system (Linux/macOS)

### Build Commands
```bash
# Build the program
make

# Clean build files
make clean

# Build and run
make run

# Debug build
make debug

# Release build
make release

# Memory check (requires valgrind)
make memcheck

# Show all available commands
make help
```

### Running the Program
```bash
./atm_system
```

## 📁 File Structure

```
atm-management-system/
├── src/                 # Source code files
├── data/               # Data storage
│   ├── users.txt       # User credentials
│   ├── records.txt     # Account records
│   └── error.log       # Error log file
├── obj/                # Object files (created during build)
├── Makefile           # Build configuration
├── README.md          # This file
└── atm_system         # Executable (created after build)
```

## 🎨 User Interface Features

### Color Coding
- **Green**: Success messages and positive values
- **Red**: Error messages and warnings
- **Blue**: Information messages
- **Yellow**: Warnings
- **Cyan**: Headers and separators
- **Bold**: Important text and prompts

### Menu Navigation
- Clear numbered options
- Input validation with retry prompts
- Consistent navigation patterns
- Professional formatting with borders

## 🔒 Security Features

- **Password Encryption**: Alphabetic mirroring encryption
- **Input Sanitization**: Prevents injection attacks
- **File Validation**: Ensures data integrity
- **Access Control**: User-specific account access
- **Error Logging**: Audit trail for debugging

## 📊 Data Validation

### Account Numbers
- Numeric only (1-18 digits)
- Uniqueness validation
- User ownership verification

### Amounts
- Range validation ($1 - $9,999,999,999)
- Decimal precision handling
- Insufficient funds checking

### Personal Information
- Name validation (alphabetic + spaces)
- Phone number format (5-14 digits)
- Country name validation
- Date format validation (MM/DD/YYYY)

## 🚨 Error Handling

### Error Types
- File operation errors
- Input validation errors
- Business logic errors
- System errors

### Error Recovery
- User-friendly error messages
- Retry options for recoverable errors
- Graceful degradation
- Automatic error logging

## 🔧 Development Features

### Debugging
- Debug build target with symbols
- Memory leak detection support
- Static analysis integration
- Code formatting tools

### Quality Assurance
- Compiler warnings enabled
- Static analysis support
- Memory checking with valgrind
- Consistent code formatting

## 📈 Performance Improvements

- Efficient file operations
- Reduced memory allocations
- Optimized string operations
- Minimal system calls

## 🎯 Future Enhancements

### Planned Features
- SQLite database integration
- Real-time notifications using pipes
- Transaction history tracking
- Account statements generation
- Multi-currency support
- Web interface

### Bonus Features Available
- Enhanced terminal UI
- Password encryption
- Comprehensive Makefile
- Error logging system
- Input validation framework

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📝 License

This project is open source and available under the MIT License.

## 🐛 Bug Reports

Please report bugs by creating an issue with:
- System information
- Steps to reproduce
- Expected vs actual behavior
- Error logs (if available)

## 📞 Support

For support and questions:
- Check the error.log file for debugging information
- Review the help command: `make help`
- Ensure all prerequisites are installed

---

**Note**: This improved version maintains backward compatibility with existing data files while providing a much better user experience and code maintainability.