{
  description = "Flake for physics engine and its demos";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { self
    , nixpkgs
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
            shellHook = /* bash */ ''
              export LD_LIBRARY_PATH=${pkgs.libxkbcommon}/lib:$LD_LIBRARY_PATH
            '';

            packages = with pkgs; [
              ninja
              cmake
              extra-cmake-modules
              cmakeCurses
              clang-tools

              SDL2
              SDL2_image
              SDL2_gfx

              sccache
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
            ] ++ libPath;
          };
      };
    });
}
