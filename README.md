## Project Structure

```
RayViz/
│
├── README.md
├── RayViz.sln
├── RayViz.vcxproj
│
├── Src/
│   │
│   ├── main.cpp
│   ├── Constants.h
|   ├── Constants.cpp
|   |
│   ├── Data_structures/
│   │   ├── Avl/
│   │   │   ├── AVL.h
│   │   │   ├── AVL.cpp
│   │   │   ├── AVLTreeVisual.h
│   │   │   ├── AVLTreeVisual.cpp
|   |   |   ├── Table.h
│   │   │   └── Table.cpp
|   |   |   
│   │   ├── Graph/
│   │   |   ├── Graph.h
│   │   |   ├── Graph.cpp
│   │   |   ├── GraphVisual.h
│   │   |   └── GraphVisual.cpp
|   |   |
│   │   ├── hash_table/
│   │   │   ├── HashTable.h
│   │   │   ├── HashTable.cpp
│   │   │   ├── HashTableVisual.h
│   │   │   └── HashTableVisual.cpp
|   |   |
│   │   └── Linked_list/
│   │       ├── LinkedList.h
│   │       ├── LinkedList.cpp
│   │       ├── LinkedListVisual.h
│   │       └── LinkedListVisual.cpp
|   |   
│   │
│   ├── UI/
│   │   ├── Components/       # Thành phần UI chung
│   │   │   ├── Button.h
│   │   │   ├── Button.cpp
│   │   │   └── TextBox.h
│   │   │
│   │   ├── screens/         # Các màn hình cụ thể
│   │   │   ├── AboutScreen.h
│   │   │   ├── AboutScreen.cpp
|   |   |   ├── Helper.h
|   |   |   ├── Helper.cpp
│   │   │   ├── SettingScreen.h
│   │   │   └── SettingScreen.cpp
│   │   │
│   │   ├── toolbar/         # Toolbar chung
│   │   │   ├── Adjust.cpp    
│   │   │   └── Adjust.h
│   │   │
│   │   ├── Menu.cpp
│   │   ├── Menu.h
│   │   ├── UI.cpp
│   │   └── UI.h
│   │
|   |
│   │
│   └── External/
│       ├── tinyfiledialogs.cpp
│       └── tinyfiledialogs.h
│
├── Assets/                  # Resources
│   ├── Images/
│   ├── Fonts/
│   └── Sound/
│
└── docs/                    # Tài liệu dự án
```

---
