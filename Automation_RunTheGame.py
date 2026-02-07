import unreal
import time
import threading
import argparse


if __name__ == "__main__":
    # --------------------------------
    # 1. Process optional arguments
    # --------------------------------
    
    parser = argparse.ArgumentParser(description='A simple script to compile blueprints and simulate the game start')

    parser.add_argument('--session_duration', '-d', type=int, default=10, help='The duration of the simulated session')
    parser.add_argument('--compile_blueprints', '-bps', action="store_true", help='The flag to start BPs compilation before starting the game')
    
    args = parser.parse_args()
    
    perform_blueprints_compilation = args.compile_blueprints
    session_duration = args.session_duration
    
    # -----------------------------
    # 2. Compile all Blueprints
    # -----------------------------

    if (perform_blueprints_compilation):
        print("Compiling all Blueprints...")

        blueprints = []
        asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()

        # Find all Blueprint assets
        blueprints = asset_registry.get_assets_by_class(unreal.TopLevelAssetPath("/Script/Engine.Blueprint"), True)

        for asset_data in blueprints:
            blueprint = asset_data.get_asset()
            if blueprint:
                unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

        print(f"Compiled {len(blueprints)} Blueprints")

    # -------------------------
    # 3. Launch game session
    # -------------------------

    print(f"Starting a game session... The session length will be {session_duration} seconds")

    unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).editor_play_simulate()

    # ----------------------
    # 4. End game session
    # ----------------------

    def delayed_action():
        print("Ending the game session...")
        unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).editor_request_end_play()

    timer = threading.Timer(10.0, delayed_action)
    timer.start()
