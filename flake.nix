{
  description = "C++26 SPR exercises with C++11 submission export";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default =
          pkgs.mkShell.override
            {
              stdenv = pkgs.clangStdenv;
            }
            {
              hardeningDisable = [
                "fortify"
                "fortify3"
              ];
              packages = with pkgs; [
                cmake
                ninja
                clang
                lldb
                gtest
                python3
              ];
            };
      }
    );
}
