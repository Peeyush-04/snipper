#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ALIAS_PATH="$SCRIPT_DIR"

# Bash/Zsh profile file
PROFILE="${HOME}/.bashrc"
if [ -n "$ZSH_VERSION" ]; then
  PROFILE="${HOME}/.zshrc"
fi

# Ensure scripts directory is in PATH
if ! grep -q "$ALIAS_PATH" "$PROFILE"; then
  echo "Adding snipper script to PATH in $PROFILE"
  echo "export PATH=\"\$PATH:$ALIAS_PATH\"" >> "$PROFILE"
  echo "Please restart your shell or run: source $PROFILE"
else
  echo "PATH already includes snipper script"
fi
