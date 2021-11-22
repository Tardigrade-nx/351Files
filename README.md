# 351Files
A single panel file Manager tailored for Anbernic 351 devices: RG351V and RG351P. Can be easily adapted to any Linux-based device.

Based on DinguxCommander.
* Original page: https://tardigrade-nx.github.io/2011/dinguxcommander

<p align="center">
  <img src="https://raw.githubusercontent.com/Tardigrade-nx/351Files/master/screenshots/01.png" alt="Screenshot 1"><br /><br />
  <img src="https://raw.githubusercontent.com/Tardigrade-nx/351Files/master/screenshots/02.png" alt="Screenshot 2"><br /><br />
  <img src="https://raw.githubusercontent.com/Tardigrade-nx/351Files/master/screenshots/03.png" alt="Screenshot 3"><br /><br />
  <img src="https://raw.githubusercontent.com/Tardigrade-nx/351Files/master/screenshots/04.png" alt="Screenshot 4"><br /><br />
</p>

# Features:
* Single panel file manager
* Copy, move, rename, delete, create directories and files.
* Display file size, compute directory size
* Text file viewer
* Text file editor
* Image viewer (original size or fit screen, next / previous image)

# Installation on 351ELEC:
351Files is now integrated in 351ELEC by default.

# Installation on ArkOS:
351Files is now integrated in ArkOS by default.

# Buttons:
* d-pad: move
* A: open / validate
* B: cancel / back
* X: open context menu
* Y: select / unselect item
* R1/R2: page down
* L1/L2: page up

Image viewer:
* d-pad: next / previous image, or scroll image
* A: switch original size / fit screen

Text editor:
* d-pad: move
* A: open virtual keyboard / validate
* B: cancel / back
* X: open context menu
* Y + d-pad: select text
* R1/R2: page down
* L1/L2: page up

# Compilation:
Define the following variables when executing 'make':
* CC
* SDL2_CONFIG
* DEVICE: CHI / RG351P / RG351V / RGB10 / RK2020 / PC
* START_PATH
* RES_PATH
