{
  description = "The flake used for spark-engine (a small 2D physics engine)";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };


  outputs =
    { nixpkgs
    , flake-utils
    , ...
    }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      libPath = with pkgs; [
        libxkbcommon
        alsa-lib
      ];
    in
    {
      devShells = {
        default = pkgs.mkShell.override
          {
            stdenv = pkgs.clang18Stdenv;
          }
          {
            name = "spark-engine";
            packages = with pkgs; [
              raylib
              ninja

              python3
              watchexec

              cmake
              extra-cmake-modules
              unzip
              doxygen
            ];

            buildInputs = with pkgs; [
              llvmPackages_18.lldb
              llvmPackages_18.clang-tools
              llvmPackages_18.clang

              pkg-config
            ];

            nativeBuildInputs = with pkgs; [
              libffi
              wayland-protocols
              wayland
              libGL

              xorg.libxcb

              xorg.libX11
              xorg.libX11
              xorg.libXrandr
              xorg.libXinerama
              xorg.libXcursor
              xorg.libXi

              libxkbcommon

              libglvnd
            ] ++ libPath;

            LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath libPath;
          };
      };
    });
}
