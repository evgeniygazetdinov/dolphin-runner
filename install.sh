#!/bin/bash

# Проверяем, установлен ли wget
if ! command -v wget &> /dev/null; then
    echo "Installing wget..."
    sudo apt-get update
    sudo apt-get install -y wget
fi

# Скачиваем установщик devkitPro
echo "Downloading devkitPro installer..."
wget https://apt.devkitpro.org/install-devkitpro-pacman

# Делаем скрипт исполняемым
chmod +x ./install-devkitpro-pacman

# Устанавливаем devkitPro
echo "Installing devkitPro..."
sudo ./install-devkitpro-pacman

# Устанавливаем пакеты для Switch
echo "Installing Switch development packages..."
sudo dkp-pacman -S --noconfirm switch-dev

# Очистка
echo "Cleaning up..."
rm ./install-devkitpro-pacman

# Экспортируем переменную DEVKITPRO
echo 'export DEVKITPRO=/opt/devkitpro' >> ~/.bashrc
export DEVKITPRO=/opt/devkitpro

echo "Installation complete!"
echo "Please restart your terminal or run: source ~/.bashrc"