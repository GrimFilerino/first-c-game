rm -rf build      
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake --build build 
cp -r ./assets ./build/assets
