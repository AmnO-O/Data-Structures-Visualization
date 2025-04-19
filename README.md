
# RayViz: Data Structures Visualization Tool

Welcome to **RayViz** – an interactive project built in C++ using the Raylib library that helps visualize fundamental data structures: **Singly Linked List**, **Hash Table**, **AVL Tree**, and **Graph**. This tool allows users to observe, interact with, and understand the behavior of these structures in real-time through engaging graphical visualizations.

## Features

- **Interactive Visualization**: Helps users understand common data structures and their operations by visualizing them step by step.
- **Data Structures Supported**:
  - **Singly Linked List**: Visualize nodes, their connections, and operations like insertion and deletion. Users can create and modify the list dynamically, updating the view in real-time.
  - **Hash Table**: Visualize the table and its operations, including insertion, removal, and collision handling using linear probing. The table dynamically updates to show the underlying changes.
  - **AVL Tree**: Display and visualize the self-balancing binary search tree. Operations like insertion, deletion, and rotations are animated to showcase how the tree maintains its balance.
  - **Graph**: Visualize graphs with nodes and edges. Users can interact by adding, removing, and modifying nodes/edges. Specialized algorithms like Kruskal’s Minimum Spanning Tree (MST) and Dijkstra's shortest path algorithm can be demonstrated on the graph.

- **User Interface**:
  - **Operations Panel**: On the left side, you can choose from various operations such as Create, Insert, Delete, Update, Search, and Clear for each data structure. This panel also allows you to initialize structures and input values dynamically.
  - **Data Structure Display**: Each section has a graphical representation of the chosen structure (e.g., the nodes and edges in the linked list, hash table slots, or AVL tree nodes). As operations are performed, the structure updates live.
  - **Graphical Control Panel**: On the bottom toolbar, users can adjust the animation speed, toggle between step-by-step or instant run modes, and zoom in or out on the display for a clearer view.
  
- **Graph Section Special Features**:
  - **Drag-and-Drop Nodes**: Easily reposition nodes to explore the graph structure.
  - **Add/Remove/Move Nodes and Edges**: Interact with the graph by clicking on nodes or edges to add, remove, or move them.
  - **Algorithm Visualization**: Watch algorithms like Dijkstra's shortest path or Kruskal's MST unfold with visual cues to help users grasp their workings.

- **Customization**:
  - **Light/Dark Mode**: Toggle between light and dark themes to suit your preferences.
  - **Background Music**: Turn music on or off to enhance focus while visualizing data structures.
  - **Responsive Interface**: The interface adapts well to different screen sizes, offering a smooth and intuitive user experience.

- **Step-by-Step Algorithm Visualization**:
  - Watch algorithm operations (e.g., insertion, traversal, rotation, searching, etc.) unfold in real-time, with live visual updates and explanations.
  - Provides informative **[INFO]** windows for deeper insights into the algorithm being visualized (e.g., AVL Tree balancing conditions, Hash Table collision handling).

## Project Structure

```
RayViz/
├── README.md               # Project documentation
├── RayViz.sln              # Solution file (Visual Studio)
├── RayViz.vcxproj          # Project configuration (Visual Studio)
├── Src/                    # Source code files
│   ├── main.cpp            # Entry point to manage overall application flow
│   ├── Constants.h         # Constants and utility functions (e.g., `random`, `drawline`)
│   ├── Constants.cpp       # Utility functions implementation
│   ├── Data_structures/    # Implementations of data structures
│   │   ├── Avl/            # AVL Tree specific files
│   │   │   ├── AVL.h       # AVL tree definitions
│   │   │   ├── AVL.cpp     # AVL tree logic implementation
│   │   │   ├── AVLTreeVisual.h # AVL visualization header
│   │   │   └── AVLTreeVisual.cpp # AVL visualization logic
│   │   ├── Graph/          # Graph specific files
│   │   │   ├── Graph.h     # Graph definitions
│   │   │   ├── Graph.cpp   # Graph logic implementation
│   │   │   ├── GraphVisual.h # Graph visualization header
│   │   │   └── GraphVisual.cpp # Graph visualization logic
│   │   ├── Hash_table/     # Hash Table specific files
│   │   │   ├── HashTable.h # Hash table definitions
│   │   │   ├── HashTable.cpp # Hash table logic implementation
│   │   │   ├── HashTableVisual.h # Hash table visualization header
│   │   │   └── HashTableVisual.cpp # Hash table visualization logic
│   │   └── Linked_list/    # Singly Linked List specific files
│   │       ├── LinkedList.h # Linked list definitions
│   │       ├── LinkedList.cpp # Linked list logic implementation
│   │       ├── LinkedListVisual.h # Linked list visualization header
│   │       └── LinkedListVisual.cpp # Linked list visualization logic
│   ├── UI/                 # User Interface files
│   │   ├── Components/     # UI components like buttons and textboxes
│   │   │   ├── Button.h    # Button class header
│   │   │   ├── Button.cpp  # Button class implementation
│   │   │   └── TextBox.h   # TextBox class header
│   │   ├── Screens/        # Screens for the interface (e.g., settings, about)
│   │   │   ├── AboutScreen.h # About screen header
│   │   │   ├── AboutScreen.cpp # About screen implementation
│   │   │   ├── SettingScreen.h # Settings screen header
│   │   │   └── SettingScreen.cpp # Settings screen implementation
│   │   ├── Toolbar/        # Toolbar controls for the UI
│   │   │   ├── Adjust.cpp  # Toolbar adjustment logic
│   │   │   └── Adjust.h    # Toolbar adjustment header
│   │   ├── Menu.cpp        # Main menu logic
│   │   ├── Menu.h          # Main menu header
│   │   ├── UI.cpp          # General UI logic
│   │   └── UI.h            # General UI header
│   └── External/           # External libraries
│       ├── tinyfiledialogs.cpp # File dialog library
│       └── tinyfiledialogs.h   # File dialog header
├── Assets/                 # Resources like images, fonts, sounds
│   ├── Images/             # Images of data structures, themes, etc.
│   ├── Fonts/              # Fonts for UI
│   ├── Toolbar/            # Toolbar images (switch on, off, pause, play)
│   └── Sound/              # Music and sound files
└── docs/                    # Documentation files
```

## Installation

### Prerequisites
- **C++ Compiler**: Ensure you have a C++14 (or higher) compatible compiler.
- **Raylib**: Download and install Raylib from [Raylib official website](https://www.raylib.com/).

### Steps

#### Option 1: Run Pre-Built Executable
1. Locate the `RayViz.exe` file in the project folder.
2. Double-click the file to launch the application.

#### Option 2: Build from Source
1. **Setup Raylib**: Make sure Raylib is correctly configured in your IDE (e.g., Code::Blocks or Visual Studio). Include the Raylib headers and link the necessary libraries.
2. Open the project in your preferred IDE.
3. Build the project.
4. Run the generated executable from your IDE.

## How to Use

1. **Launch the app** by running the executable.
2. The graphical interface will display the available data structures: Linked List, Hash Table, AVL Tree, and Graph.
3. **Select a Data Structure** to visualize:
   - Choose the structure from the main menu (Linked List, Hash Table, AVL Tree, or Graph).
   - Watch as operations like insertion, deletion, and traversal unfold visually.
4. **Interactive Controls**:
   - Add, delete, and modify elements using intuitive controls.
   - Step through each operation with a single click.
5. **Graph Section Special Features**:
   - **Drag-and-Drop**: Reposition nodes freely by dragging them.
   - **Click to Modify**: Add, remove, or move nodes and edges by clicking.
   - **Animated Algorithm Execution**: Watch operations like search or sort unfold with visual cues.
6. **Customization**:
   - Switch between **Light** and **Dark** modes for the best viewing experience.
   - Adjust themes and node designs to suit your preferences.
7. **Sound**: Turn sound effects on/off and control background music volume to enhance your focus or mood while using the app.

## Project Dependencies

- **Raylib**: Used for rendering graphics and handling user input.
- **Standard C++ Libraries**: For general core functionality.
- **TinyFileDialogs**: For file dialogs to load/save data structures.

## Known Issues

- The application is constrained to a fixed window size; fullscreen support is not yet available.

## Future Features

- **Multiplayer**: Add the ability to collaborate or compete in visualizing and manipulating data structures.
- **Enhanced Visuals**: Further improve graphical rendering, animations, and smoother transitions between operations.
- **User-defined Data Structures**: Allow users to input their own data structures to visualize.

## Contribution

Contributions are welcome! Feel free to submit pull requests, report issues, or suggest improvements.

## Contact

For questions or support, please reach out via:

- Email: khln.apcs@gmail.comcom

