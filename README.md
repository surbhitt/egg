<p align="center">
    <img src="https://raw.githubusercontent.com/surbhitt/egg/main/assets/egg.png"/ height=35% width=35%>
</p>

# Introduction 

Engine for game and graphics. A single-file/header-only library in C++ for creating graphics; with extension for implementing a game loop. Utilising the terminals in-built capability to render truecolor (xterm-256color). 

[NOTE]
check dev branch porting to gcc from msvc

The code was written on a windows machine with the target as windows, compiled using the MSVC build tools. In the process of porting to gcc and making the code linux compatible. 


[GOALS to Implement]
- Sprite generation
- Physics engine
- Audio Support


# Usage

Add the header file to your project. 
In your project file give defination to the virtual functions 

main should look like this
```cpp
	int main()
	{
		EGG game;

		// Create a console with resolution 160x100 characters
		// Each character occupies 8x8 pixels
		game.ConstructConsole(160, 100, 8, 8);

		// Start the engine!
		game.Start();

		return 0;
	}
```
