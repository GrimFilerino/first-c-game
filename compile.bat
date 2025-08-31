rm -rf build      
cmake -S . -B build
cmake --build build 
cp -r ./assets ./build/assets
