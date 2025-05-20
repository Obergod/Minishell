#!/bin/bash

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Logo
print_logo() {
    echo -e "${CYAN}"
    cat << "EOF"
   ██████╗██╗   ██╗██████╗ ███████╗ ██████╗ ██████╗      ██████╗ ██████╗  ██████╗   
  ██╔════╝██║   ██║██╔══██╗██╔════╝██╔═══██╗██╔══██╗     ██╔══██╗██╔══██╗██╔═══██╗  
  ██║     ██║   ██║██████╔╝███████╗██║   ██║██████╔╝     ██████╔╝██████╔╝██║   ██║  
  ██║     ██║   ██║██╔══██╗╚════██║██║   ██║██╔══██╗     ██╔═══╝ ██╔══██╗██║   ██║  
  ╚██████╗╚██████╔╝██║  ██║███████║╚██████╔╝██║  ██║     ██║     ██║  ██║╚██████╔╝  
   ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝     ╚═╝     ╚═╝  ╚═╝ ╚═════╝  
EOF
    echo -e "${NC}"
}


# Get download folder path
get_downloads_dir() {
    if [[ "$(uname)" == "Darwin" ]]; then
        echo "$HOME/Downloads"
    else
        if [ -f "$HOME/.config/user-dirs.dirs" ]; then
            . "$HOME/.config/user-dirs.dirs"
            echo "${XDG_DOWNLOAD_DIR:-$HOME/Downloads}"
        else
            echo "$HOME/Downloads"
        fi
    fi
}

# Get latest version
get_latest_version() {
    echo -e "${CYAN}ℹ️ Checking latest version...${NC}"
    local latest_release
    latest_release=$(curl -s https://api.github.com/repos/yeongpin/cursor-free-vip/releases/latest) || {
        echo -e "${RED}❌ Cannot get latest version information${NC}"
        exit 1
    }
    
    VERSION=$(echo "$latest_release" | grep -o '"tag_name": ".*"' | cut -d'"' -f4 | tr -d 'v')
    if [ -z "$VERSION" ]; then
        echo -e "${RED}❌ Failed to parse version from GitHub API response:\n${latest_release}"
        exit 1
    fi

    echo -e "${GREEN}✅ Found latest version: ${VERSION}${NC}"
}

# Detect system type and architecture
detect_os() {
    case "$(uname)" in
        Darwin)
            ARCH=$(uname -m)
            if [[ "$ARCH" == "arm64" ]]; then
                OS="mac_arm64"
                echo -e "${CYAN}ℹ️ Detected macOS ARM64 architecture${NC}"
            else
                OS="mac_intel"
                echo -e "${CYAN}ℹ️ Detected macOS Intel architecture${NC}"
            fi
            ;;
        Linux)
            ARCH=$(uname -m)
            if [[ "$ARCH" == "aarch64" || "$ARCH" == "arm64" ]]; then
                OS="linux_arm64"
                echo -e "${CYAN}ℹ️ Detected Linux ARM64 architecture${NC}"
            else
                OS="linux_x64"
                echo -e "${CYAN}ℹ️ Detected Linux x64 architecture${NC}"
            fi
            ;;
        *)
            OS="windows"
            echo -e "${CYAN}ℹ️ Detected Windows system${NC}"
            ;;
    esac
}

# Install and download without sudo
install_cursor_free_vip() {
    local downloads_dir
    downloads_dir=$(get_downloads_dir)
    local binary_name="CursorFreeVIP_${VERSION}_${OS}"
    local binary_path="${downloads_dir}/${binary_name}"
    local download_url="https://github.com/yeongpin/cursor-free-vip/releases/download/v${VERSION}/${binary_name}"
    
    # If already downloaded
    if [ -f "$binary_path" ]; then
        echo -e "${GREEN}✅ Found existing installation file${NC}"
        echo -e "${CYAN}ℹ️ Location: ${binary_path}${NC}"

        echo -e "${CYAN}ℹ️ Setting executable permissions...${NC}"
        if chmod +x "$binary_path"; then
            echo -e "${GREEN}✅ Permissions set${NC}"
        else
            echo -e "${RED}❌ Failed to set permissions. Please check file ownership.${NC}"
            exit 1
        fi

        echo -e "${CYAN}ℹ️ Starting program...${NC}"
        "$binary_path"
        return
    fi
    
    echo -e "${CYAN}ℹ️ No existing installation file found, starting download...${NC}"
    echo -e "${CYAN}ℹ️ Downloading to ${downloads_dir}...${NC}"
    echo -e "${CYAN}ℹ️ Download link: ${download_url}${NC}"
    
    # Check existence of URL
    if curl --output /dev/null --silent --head --fail "$download_url"; then
        echo -e "${GREEN}✅ File exists, starting download...${NC}"
    else
        echo -e "${RED}❌ Download link does not exist: ${download_url}${NC}"
        echo -e "${YELLOW}⚠️ Trying without architecture suffix...${NC}"
        
        # Fallback without arch
        OS_SIMPLE=${OS%%_*}
        binary_name="CursorFreeVIP_${VERSION}_${OS_SIMPLE}"
        download_url="https://github.com/yeongpin/cursor-free-vip/releases/download/v${VERSION}/${binary_name}"
        echo -e "${CYAN}ℹ️ New download link: ${download_url}${NC}"

        if ! curl --output /dev/null --silent --head --fail "$download_url"; then
            echo -e "${RED}❌ New download link does not exist${NC}"
            exit 1
        fi
    fi
    
    # Perform download
    if ! curl -L -o "$binary_path" "$download_url"; then
        echo -e "${RED}❌ Download failed${NC}"
        exit 1
    fi
    
    # Verify size
    local file_size
    file_size=$(stat -c%s "$binary_path" 2>/dev/null || stat -f%z "$binary_path" 2>/dev/null)
    echo -e "${CYAN}ℹ️ Downloaded file size: ${file_size} bytes${NC}"
    
    if [ "$file_size" -lt 1000 ]; then
        echo -e "${YELLOW}⚠️ Warning: Downloaded file is too small, possibly invalid${NC}"
        cat "$binary_path"
        echo -e "${RED}❌ Installation failed, please check version and OS${NC}"
        exit 1
    fi
    
    echo -e "${CYAN}ℹ️ Setting executable permissions...${NC}"
    if chmod +x "$binary_path"; then
        echo -e "${GREEN}✅ Installation completed!${NC}"
        echo -e "${CYAN}ℹ️ Program downloaded to: ${binary_path}${NC}"
        echo -e "${CYAN}ℹ️ Starting program...${NC}"
        "$binary_path"
    else
        echo -e "${RED}❌ Installation failed (permission error)${NC}"
        exit 1
    fi
}

# Main
main() {
    print_logo
    get_latest_version
    detect_os
    install_cursor_free_vip
}

main

