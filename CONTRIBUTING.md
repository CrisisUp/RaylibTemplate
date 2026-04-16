# Guia de Contribuição

Obrigado pelo seu interesse em contribuir para o **Ralph Arcade**! Este projeto segue padrões profissionais de desenvolvimento em C++.

## 🛠️ Requisitos de Desenvolvimento

Antes de começar, certifique-se de ter as seguintes ferramentas:

- **Compilador:** GCC 11+, Clang 13+, ou MSVC 2019+
- **CMake:** Versão 3.15 ou superior
- **Doxygen:** (Opcional) Para gerar documentação
- **Clang-Format & Clang-Tidy:** Para garantir a consistência do código

## 📝 Padrões de Código

Seguimos as seguintes regras para manter a base de código limpa:

1.  **Formatação:** Sempre execute o `clang-format` nos arquivos antes de commitar.
2.  **Nomenclatura:**
    - Classes e Structs: `PascalCase` (ex: `GameplayScene`)
    - Métodos e Funções: `PascalCase` (ex: `UpdateMusicStream`)
    - Variáveis Membro (Privadas): `camelCase` (ex: `isPaused`)
3.  **Includes:** Use caminhos relativos ao diretório `include/` (ex: `#include "Game.h"`).

## 🧪 Testes Unitários

Não aceitamos PRs sem testes para novas funcionalidades de lógica. Usamos o **GoogleTest**.

Para rodar os testes:
```bash
mkdir -p build && cd build
cmake ..
make unit_tests
./unit_tests
```

## 🔍 Profiling e Performance

Este projeto integra o **Tracy Profiler**. Para analisar a performance:
1. Compile com `TRACY_ENABLE=ON` (padrão em Debug).
2. Abra o executável do Tracy para conectar ao jogo.

## 🚀 Fluxo de Trabalho (Git)

1. Faça um **Fork** do projeto.
2. Crie uma **Branch** para sua funcionalidade (`git checkout -b feature/nova-mecanica`).
3. Commit suas mudanças com mensagens claras (ex: `feat: add pause system`).
4. Envie o **Pull Request**.

---
*Mantenha o código simples, eficiente e bem documentado!*
