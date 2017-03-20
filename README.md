# Origami-flight

This project display an animation with an origami folding by itself and flighing away. It has been created with **povray** by Chloé Maroteaux and Hugo Rollin.

To compile, run in your unix terminal :

```shell
mkdir img
povray +L/usr/local/share/povray-3.7/include/ +Iscene.pov +W1200 +H800 +10.3 -V -GA +KFI1 +KFF320 +KI1 +Oimg +FJ
ffmpeg -framerate 10 -i img%03d.jpg -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p result.mp4
```
  
You might want to change the include directory if you didn't install povray with the package manager.

You can run a low definition compilation with :

```shell
povray +L/usr/local/share/povray-3.7/include/ +Iscene.pov +W800 +H600 -A -V -GA +KFI1 +KFF320 +KI1 +Oimg +FJ
```
