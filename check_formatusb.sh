#!/bin/bash

###############################################################################
# FormatUSB Health Check Script v1.0.0.3
# Script untuk mengecek kesehatan dan kelengkapan aplikasi FormatUSB
# Copyright (C) 2025 FormatUSB Team
###############################################################################

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_CHECKS=0
PASSED_CHECKS=0
FAILED_CHECKS=0
WARNING_CHECKS=0

# Functions
print_header() {
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  FormatUSB Health Check - Version 1.0.0.3${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo ""
}

print_section() {
    echo -e "\n${BLUE}▶ $1${NC}"
    echo "─────────────────────────────────────────────────────────"
}

check_pass() {
    TOTAL_CHECKS=$((TOTAL_CHECKS + 1))
    PASSED_CHECKS=$((PASSED_CHECKS + 1))
    echo -e "  ${GREEN}✓${NC} $1"
}

check_fail() {
    TOTAL_CHECKS=$((TOTAL_CHECKS + 1))
    FAILED_CHECKS=$((FAILED_CHECKS + 1))
    echo -e "  ${RED}✗${NC} $1"
}

check_warn() {
    TOTAL_CHECKS=$((TOTAL_CHECKS + 1))
    WARNING_CHECKS=$((WARNING_CHECKS + 1))
    echo -e "  ${YELLOW}⚠${NC} $1"
}

print_summary() {
    echo ""
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  Summary${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "  Total checks: ${TOTAL_CHECKS}"
    echo -e "  ${GREEN}Passed: ${PASSED_CHECKS}${NC}"
    echo -e "  ${YELLOW}Warnings: ${WARNING_CHECKS}${NC}"
    echo -e "  ${RED}Failed: ${FAILED_CHECKS}${NC}"
    echo ""
    
    if [ $FAILED_CHECKS -eq 0 ] && [ $WARNING_CHECKS -eq 0 ]; then
        echo -e "${GREEN}✓ FormatUSB is in excellent condition!${NC}"
        return 0
    elif [ $FAILED_CHECKS -eq 0 ]; then
        echo -e "${YELLOW}⚠ FormatUSB is functional but has some warnings${NC}"
        return 1
    else
        echo -e "${RED}✗ FormatUSB has critical issues that need attention${NC}"
        return 2
    fi
}

# Start checks
print_header

# 1. Check Core Source Files
print_section "1. Checking Core Source Files"

required_files=(
    "main.cpp"
    "mainwindow.cpp"
    "mainwindow.h"
    "mainwindow.ui"
    "about.cpp"
    "about.h"
    "cmd.cpp"
    "cmd.h"
    "version.h"
)

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        check_pass "Found: $file"
    else
        check_fail "Missing: $file"
    fi
done

# 2. Check Project Configuration Files
print_section "2. Checking Project Configuration"

if [ -f "src.pro" ]; then
    check_pass "Found: src.pro (Qt project file)"
    
    # Check if version in src.pro is correct
    if grep -q "1.0.0.3" src.pro; then
        check_pass "Version 1.0.0.3 in src.pro"
    else
        check_warn "Version mismatch in src.pro"
    fi
else
    check_fail "Missing: src.pro"
fi

if [ -f "Makefile" ]; then
    check_pass "Found: Makefile (generated)"
else
    check_warn "Missing: Makefile (run 'qmake src.pro' to generate)"
fi

# 3. Check Resource Files
print_section "3. Checking Resource Files"

if [ -f "images.qrc" ]; then
    check_pass "Found: images.qrc (Qt resource file)"
    
    # Check if CHANGELOG is embedded
    if grep -q "CHANGELOG.txt" images.qrc; then
        check_pass "CHANGELOG.txt embedded in resources"
    else
        check_fail "CHANGELOG.txt not embedded in resources"
    fi
else
    check_fail "Missing: images.qrc"
fi

if [ -f "CHANGELOG.txt" ]; then
    check_pass "Found: CHANGELOG.txt"
    
    # Check version in CHANGELOG
    if grep -q "1.0.0.3" CHANGELOG.txt; then
        check_pass "Version 1.0.0.3 in CHANGELOG.txt"
    else
        check_warn "Version mismatch in CHANGELOG.txt"
    fi
else
    check_warn "Missing: CHANGELOG.txt"
fi

# 4. Check Library Files
print_section "4. Checking Library Files"

if [ -f "lib/formatusb_lib" ]; then
    check_pass "Found: lib/formatusb_lib"
    
    # Check if executable
    if [ -x "lib/formatusb_lib" ]; then
        check_pass "lib/formatusb_lib is executable"
    else
        check_warn "lib/formatusb_lib is not executable (run 'chmod +x lib/formatusb_lib')"
    fi
    
    # Check version in lib
    if grep -q "1.0.0.3" lib/formatusb_lib; then
        check_pass "Version 1.0.0.3 in lib/formatusb_lib"
    else
        check_warn "Version mismatch in lib/formatusb_lib"
    fi
else
    check_fail "Missing: lib/formatusb_lib"
fi

# 5. Check Translation Files
print_section "5. Checking Translation Files"

translation_count=$(find translations -name "*.ts" 2>/dev/null | wc -l)
if [ $translation_count -gt 0 ]; then
    check_pass "Found: $translation_count translation files"
else
    check_warn "No translation files found in translations/"
fi

# 6. Check Documentation
print_section "6. Checking Documentation"

doc_files=("README.md" "README1.md" "replit.md" "LICENSE")
for file in "${doc_files[@]}"; do
    if [ -f "$file" ]; then
        check_pass "Found: $file"
        
        # Check version in documentation
        if [[ "$file" == "README"* ]]; then
            if grep -q "1.0.0.3" "$file"; then
                check_pass "Version 1.0.0.3 in $file"
            else
                check_warn "Version mismatch in $file"
            fi
        fi
    else
        check_warn "Missing: $file"
    fi
done

# 7. Check Debian Packaging
print_section "7. Checking Debian Package Files"

if [ -d "debian" ]; then
    check_pass "Found: debian/ directory"
    
    debian_files=("changelog" "control" "copyright" "rules")
    for file in "${debian_files[@]}"; do
        if [ -f "debian/$file" ]; then
            check_pass "Found: debian/$file"
        else
            check_warn "Missing: debian/$file"
        fi
    done
    
    # Check version in debian/changelog
    if [ -f "debian/changelog" ]; then
        if grep -q "1.0.0.3" debian/changelog; then
            check_pass "Version 1.0.0.3 in debian/changelog"
        else
            check_warn "Version mismatch in debian/changelog"
        fi
    fi
else
    check_warn "Missing: debian/ directory"
fi

# 8. Check Build Dependencies
print_section "8. Checking Build Dependencies"

dependencies=(
    "qmake:Qt build system"
    "make:Build automation"
    "g++:C++ compiler"
    "pkg-config:Package configuration"
)

for dep_info in "${dependencies[@]}"; do
    dep="${dep_info%%:*}"
    desc="${dep_info##*:}"
    
    if command -v "$dep" >/dev/null 2>&1; then
        version=$($dep --version 2>&1 | head -n1 || echo "unknown")
        check_pass "$desc ($dep) - installed"
    else
        check_warn "$desc ($dep) - not found"
    fi
done

# 9. Check Executable
print_section "9. Checking Compiled Executable"

if [ -f "formatusb" ]; then
    check_pass "Found: formatusb executable"
    
    # Check if executable
    if [ -x "formatusb" ]; then
        check_pass "formatusb is executable"
    else
        check_fail "formatusb is not executable"
    fi
    
    # Check file size
    size=$(stat -f%z "formatusb" 2>/dev/null || stat -c%s "formatusb" 2>/dev/null)
    if [ -n "$size" ] && [ "$size" -gt 10000 ]; then
        check_pass "formatusb size: $(numfmt --to=iec-i --suffix=B $size 2>/dev/null || echo "$size bytes")"
    else
        check_warn "formatusb size is suspiciously small"
    fi
    
    # Try to check version
    if ./formatusb --version 2>&1 | grep -q "1.0.0.3"; then
        check_pass "formatusb reports correct version: 1.0.0.3"
    else
        check_warn "formatusb version check failed or mismatch"
    fi
else
    check_warn "formatusb executable not found (run 'make' to build)"
fi

# 10. Check for Common Issues
print_section "10. Checking for Common Issues"

# Check for backup files
backup_count=$(find . -name "*.bak" -o -name "*~" 2>/dev/null | wc -l)
if [ $backup_count -eq 0 ]; then
    check_pass "No backup files found"
else
    check_warn "Found $backup_count backup files (.bak or ~)"
fi

# Check for .o files
obj_count=$(find . -name "*.o" 2>/dev/null | wc -l)
if [ $obj_count -eq 0 ]; then
    check_pass "No stale object files"
else
    echo -e "  ${YELLOW}ℹ${NC} Found $obj_count object files (normal after build)"
fi

# Check for moc files
moc_count=$(find . -name "moc_*.cpp" 2>/dev/null | wc -l)
if [ $moc_count -gt 0 ]; then
    echo -e "  ${YELLOW}ℹ${NC} Found $moc_count Qt meta-object files (normal after build)"
fi

# 11. Version Consistency Check
print_section "11. Version Consistency Check"

version_files=(
    "version.h"
    "mainwindow.ui"
    "src.pro"
    "lib/formatusb_lib"
    "CHANGELOG.txt"
    "debian/changelog"
    "README.md"
    "README1.md"
)

inconsistent_versions=0
for file in "${version_files[@]}"; do
    if [ -f "$file" ]; then
        if grep -q "1.0.0.3" "$file"; then
            : # Version is correct
        else
            check_warn "Version inconsistency in: $file"
            inconsistent_versions=$((inconsistent_versions + 1))
        fi
    fi
done

if [ $inconsistent_versions -eq 0 ]; then
    check_pass "All version references are consistent (1.0.0.3)"
fi

# 12. File Permissions Check
print_section "12. File Permissions Check"

# Check if script files are executable
scripts=("lib/formatusb_lib")
for script in "${scripts[@]}"; do
    if [ -f "$script" ]; then
        if [ -x "$script" ]; then
            check_pass "$script has execute permission"
        else
            check_warn "$script missing execute permission"
        fi
    fi
done

# 13. Directory Structure Check
print_section "13. Directory Structure Check"

required_dirs=("lib" "translations" "images" "debian")
for dir in "${required_dirs[@]}"; do
    if [ -d "$dir" ]; then
        count=$(find "$dir" -type f 2>/dev/null | wc -l)
        check_pass "Directory $dir/ exists with $count files"
    else
        check_warn "Directory $dir/ is missing"
    fi
done

# Print final summary
print_summary
exit_code=$?

echo ""
echo "Run 'bash check_formatusb.sh' anytime to recheck the project health."
echo ""

exit $exit_code
