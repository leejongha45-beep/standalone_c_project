# Project-local Clang PATH for /home/ubuntu/projects/cp.
# Source once in an existing zsh; new interactive shells load this via .zshrc.
_cp_project_clang_path() {
    emulate -L zsh
    local cp_root=/home/ubuntu/projects/cp
    local cp_bin="$cp_root/.toolchain/bin"
    if [[ "${PWD:A}" == "$cp_root" || "${PWD:A}" == "$cp_root"/* ]]; then
        if [[ "${path[1]-}" != "$cp_bin" ]]; then
            path=("$cp_bin" "${(@)path:#$cp_bin}")
        fi
    elif (( ${path[(Ie)$cp_bin]} )); then
        path=("${(@)path:#$cp_bin}")
    fi
    export PATH
}
autoload -Uz add-zsh-hook
add-zsh-hook -d chpwd _cp_project_clang_path
add-zsh-hook chpwd _cp_project_clang_path
_cp_project_clang_path
