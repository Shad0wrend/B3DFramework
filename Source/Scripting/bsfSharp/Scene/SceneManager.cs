//********************************* B3D Framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;

namespace b3d
{
    /** @addtogroup Scene
     *  @{
     */
    public partial class SceneManager
    {
        private static RRef<Scene> activateOnLoadScene;

        /// <summary>
        /// Loads a new scene asynchronously.
        /// </summary>
        /// <param name="path">Path to the prefab to load.</param>
        /// <returns>Handle to the prefab of the scene at the provided path.</returns>
        public static RRef<Scene> LoadMainSceneAsync(string path)
        {
            ClearMainScene();

            activateOnLoadScene = Resources.LoadAsReference<Scene>(path);

            if (activateOnLoadScene != null && activateOnLoadScene.IsLoaded)
            {
                SetActiveMainScene(activateOnLoadScene.Value);
                activateOnLoadScene = null;
            }

            return activateOnLoadScene;
        }


        /// <summary>
        /// Clears all scene objects from the current scene.
        /// </summary>
        public static void ClearMainScene()
        {
            activateOnLoadScene = null;
            Internal_ClearMainScene(false);
        }


        /// <summary>
        /// Called once per frame by the runtime.
        /// </summary>
        internal static void Update()
        {
            if (activateOnLoadScene != null && activateOnLoadScene.IsLoaded)
            {
                SetActiveMainScene(activateOnLoadScene.Value);
                activateOnLoadScene = null;
            }
        }

        /// <summary>
        /// Makes the provided scene resource the current main scene instance.
        /// </summary>
        /// <param name="scene">Previously loaded scene resource.</param>
        private static void SetActiveMainScene(Scene scene)
        {
            if(scene != null)
            {
                LoadMainScene(scene);
            }
            else
            {
                Debug.LogError("Attempting to activate a scene that hasn't finished loading yet.", "Scene");
            }
        }
    }

    /** @} */
}
