# Installation Guide

**Prerequisites:**

- **Unreal Engine 5.3 Installed:** Ensure you have Unreal Engine 5.3 downloaded and installed on your system. You can get it from the official Unreal Engine website: [https://www.unrealengine.com/en-US/download](https://www.unrealengine.com/en-US/download)
- **Visual Studio 2022 Installed:** Make sure you have Visual Studio 2022 (Community, Professional, or Enterprise edition) installed on your computer. You can download it from the official Microsoft website: [https://visualstudio.microsoft.com/downloads/](https://visualstudio.microsoft.com/downloads/)

**Building the Plugin:**

1. **Create a New C++ Project in Unreal Engine:**
   - Open Unreal Engine and create a new C++ project. Choose a name for your project (e.g., WatsonxVR) and select "**Empty Project**" as the template. Ensure the Engine Version matches your Unreal Engine installation (5.3 in this case).

2. **Set Up the Plugin Directory:**
   - Inside your newly created C++ project, create a folder named "Plugins" at the root level. This folder will house your plugin.

3. **Copy the WatsonXVR Plugin Files:**
   - Copy the entire `WatsonxVR` folder (containing `WatsonxVR.uplugin`, `Source`, and `Resources`) from your original source code into the `Plugins` folder you created within your Unreal Engine project.

4. **Generate Visual Studio Project Files:**
   - Right-click on your project's `.uproject` file in the Unreal Editor and select "**Generate Visual Studio project files**."

5. **Open the Project in Visual Studio:**
   - Double-click on the generated `.sln` file (usually named after your project) to open it in Visual Studio 2022.

6. **Configure Build Settings (Optional):**
   - In Visual Studio, you can navigate to the project's properties (right-click on the project name in the Solution Explorer and select "Properties") and adjust build configurations (e.g., Debug, Release) if needed. However, for basic plugin building, the default settings should suffice.

7. **Build the Plugin:**
   - In Visual Studio, navigate to the "Build" menu and select "**Build Solution**." This will compile the plugin code.

**Installing the Plugin in Unreal Engine 5.3:**

1. **Close and Relaunch Unreal Engine:**
   - After a successful build, close and relaunch Unreal Engine 5.3.

2. **Enable the Plugin:**
   - Open the "Edit" menu in the Unreal Editor, select "**Plugins**," and browse through the plugin list. Find the "**HttpGPT**" plugin and check the box next to it to enable it.

**Verifying Installation:**

1. **Create a New Project or Open an Existing One:**
   - Launch a new Unreal Engine project or open an existing one where you want to use the plugin.

2. **Check the Content Browser:**
   - In the Content Browser, navigate to the "Plugins" folder. If the installation was successful, you should see a folder named "**WatsonxVR**" or a similar name based on your plugin configuration.

That's it! You've successfully built and installed the WatsonXVR plugin in Unreal Engine 5.3 using Visual Studio 2022. Now you can start using the plugin's functionality within your Unreal Engine projects.

**Additional Notes:**

- If you encounter any build errors during the process, double-check that the Unreal Engine version in your C++ project matches your installed Unreal Engine and that all the dependencies mentioned in the `WatsonxVR.Build.cs` file are correctly linked.
- You might need to adjust the build configuration settings in Visual Studio depending on your specific requirements (e.g., Debug vs. Release build).
- Refer to the Unreal Engine documentation for more advanced plugin development topics: [https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/Plugins](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/Plugins)