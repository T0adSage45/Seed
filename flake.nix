{
  description = "Seed's environment setup";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/4c1018dae018162ec878d42fec712642d214fdfa";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      {
        devShells.default = pkgs.mkShell {
          name = "Seed-environment";

          packages = with pkgs; [
            cmake
            gnumake
            bear
            sdl3
            perf

            clang
            clang-tools
            libcxx
            lld
            lldb

            pkg-config

            libGL
            glew
            glm
            mesa

            # Vulkan
            vulkan-loader
            vulkan-headers
            vulkan-tools
            vulkan-validation-layers

            # Optional helper library
            # glfw

            # Optional shader compiler
            # shaderc
          ];

          shellHook = ''
            echo "Seed development Env"
            echo "Compiler: $(clang++ --version | head -n1)"

            export CC=clang
            export CXX=clang++

            # Vulkan SDK paths
            export VK_LAYER_PATH=${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d

            exec zsh
          '';
        };
      }
    );
}
