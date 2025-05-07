{
  description = "Flake for physics engine and its demos";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs {
          inherit system;
        };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell.override
          {
            stdenv = pkgs.clang18Stdenv;
          }
          {
            shellHook = /* bash */ ''
              export LD_LIBRARY_PATH=${pkgs.libxkbcommon}/lib:$LD_LIBRARY_PATH
            '';

            packages = with pkgs; [
              ninja
              cmake
              extra-cmake-modules
              cmakeCurses
              clang-tools
            ];

            buildInputs = with pkgs; [
              clang
              libffi
            ];

            nativeBuildInputs = with pkgs; [
              SDL2
              SDL2_image
              SDL2_gfx

              glew
              glm
            ];
          };
      });
    };
}
