//
//  main.cpp
//  gl-learning
//
//  Created by mrichmon on 10/9/24.
//

#include "Instance.cpp"
#include <iostream>
#include <swivel_chair/fileManager.h>

int main() {
    Instance inst = Instance();
    inst.init(FileSystem::getPath("simOptions.json"));
    inst.start();
    inst.destroy();
    return 0;
}