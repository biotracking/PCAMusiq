PCAMusiq
========
Tools for doing PCA on video and making music

Building PCAVideoPlayer
-----------------------
```sh
./InstallBuildDependenciesOSX.sh
```
PCAVideoPlayer uses a Qt-based build (qmake). You can open the .pro file in QtCreator or you can use qmake on the command line.

Running PCAVideoPlayer
-----------------------
```sh
path/to/PCAMusiqPlayer <original_video> <pca_directory> <osc_address_and_port>
```
for example:
```
./PCAMusiqPlayer-build-desktop-Qt_4_8_3_in_PATH__System__Debug/PCAMusiqPlayer.app/Contents/MacOS/PCAMusiqPlayer ../AquariumData/Videos/aquarium.mov ../AquariumData/PCAVideoBases/bwbasis_every-1_PCA 127.0.0.1:50000
```
