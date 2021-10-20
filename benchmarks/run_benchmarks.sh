echo "Compressing linux_exe"
time ./MAIN -c linux_exe_arch linux_exe
echo "Decompressing linux_exe_arch"
time ./MAIN -d linux_exe_arch
echo "Compressing lorem"
time ./MAIN -c lorem_arch lorem
echo "Decompressing lorem_arch"
time ./MAIN -d lorem_arch
echo "Compressing image"
time ./MAIN -c image_arch space.jpg
echo "Decompressing image_arch"
time ./MAIN -d image_arch
echo "Compressing video"
time ./MAIN -c video_arch Pexels\ Videos\ 1851190.mp4
echo "Decompressing video_arch"
time ./MAIN -d video_arch
rm *_arch
