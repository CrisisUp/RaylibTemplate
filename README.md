# Ralph Arcade - Legendary Repaired Edition

Bem-vindo ao **Ralph Arcade**, um jogo de ação frenética em estilo retro construído com a biblioteca Raylib. Você controla o Ralph e deve sobreviver em mundos gerados proceduralmente enquanto busca a pontuação máxima!

## 🕹️ Como Jogar

O objetivo é simples: **colete todas as moedas amarelas** para avançar de nível. Mas cuidado, os inimigos e o cenário ficarão mais perigosos a cada passo!

### Controles (Totalmente Configuráveis!)

* **Setas do Teclado:** Movimentam o Ralph (Padrão).
* **Barra de Espaço:** Ativa o **DASH** (Impulso rápido).
* **Enter:** Inicia o jogo ou avança entre telas (Menu, Level Up, Game Over).
* **O:** Abre o menu de **Opções** para ajustar volumes, idioma e **RECONFIGURAR TECLAS**.
* **Gamepad:** Suporte nativo a controles (Xbox/Playstation/Generic).

---

## 🛠️ Funcionalidades de Engenharia (Elite)

Este projeto foi atualizado com padrões de excelência técnica:

### 1. Acessibilidade (Input Rebinding)

* Sistema de entrada flexível com suporte a múltiplas associações.
* Interface visual no menu de Opções para remapear teclas em tempo real.
* Detecção automática de Gamepad com mapeamento dinâmico.

### 2. Documentação Técnica Automática (Doxygen)

* Gere a documentação completa do código fonte (classes, métodos, arquitetura).
* Requer **Doxygen** instalado no sistema.
* Comando: `cd build && make docs` (gera em `build/docs/html`).

### 3. Métricas de Qualidade (Code Coverage)

* Suporte a relatórios de cobertura de testes (GCC/Clang).
* Habilite via CMake: `cmake .. -DENABLE_COVERAGE=ON`.
* Execute os testes: `make unit_tests && ./unit_tests`.
* Gere o relatório (ex: `lcov`): `lcov --capture --directory . --output-file coverage.info`.

---

## 🚀 Mecânicas Principais

... (resto do arquivo)

### 1. Sistema de Níveis e Biomas

* O mapa é gerado aleatoriamente a cada nível (Roguelike!).
* **Biomas:** O cenário muda visualmente a cada 5 níveis (Deserto, Floresta e Caverna).
* **Progressão:** A velocidade dos inimigos aumenta em **2%** a cada novo nível.

### 2. Combos e Pontuação

* **Combo:** Colete moedas em sucessão rápida (menos de 2 segundos) para ativar o multiplicador.
* Quanto maior o combo, maior o bônus de pontos por moeda (x2, x3, x4...).
* **High Score:** Sua melhor pontuação fica salva automaticamente no arquivo `highscore.txt`.

### 3. Power-Ups (Estrelas Azuis)

De tempos em tempos, uma estrela de poder surge no mapa:

* 🟡 **SPEED BOOST:** Ralph corre 2x mais rápido.
* 🔵 **INVINCIBILITY:** Ralph pode atropelar inimigos para ganhar +50 pontos.
* 🟢 **SLOW MOTION:** Inimigos movem-se a 40% da velocidade original.

### 4. Vidas e Dano

* Você começa com **3 vidas** (representadas pelas cabeças do Ralph no topo).
* Ao ser atingido, você ganha **1.5s de invulnerabilidade** (o Ralph ficará piscando).
* **Feedback de Elite:** O jogo possui efeitos de *Impact Freeze* (congelamento), *Screen Shake* (tremor) e *Flash* ao sofrer ou causar dano.

### 5. O Grande Chefe (Nível 10)

* Ao chegar no **Nível 10**, você enfrentará um **Boss Gigante**.
* Ele possui 10 pontos de vida.
* **Ataque:** Para causar dano ao Chefe, você deve atingi-lo enquanto estiver **Invencível** ou usando o **Dash**.

---

## 🖥️ Requisitos e Compilação

Este projeto utiliza **CMake** e **FetchContent** para gerenciar dependências automaticamente (Raylib, spdlog, Tracy, GTest). No entanto, algumas bibliotecas do sistema são necessárias dependendo da sua plataforma:

### 🐧 Linux (Ubuntu/Debian)

Você precisará das bibliotecas de desenvolvimento para X11, OpenGL e Áudio:

```bash
sudo apt-get update
sudo apt-get install build-essential git cmake
sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
```

### 🍎 macOS

Recomendamos o uso do **Homebrew**:

```bash
brew install cmake raylib
```

### 🪟 Windows

* Recomendamos o **Visual Studio 2022** (com C++ Desktop Development) ou **MinGW-w64**.
* O CMake baixará e compilará a Raylib automaticamente durante a primeira build.

---

## 🛠️ Como Compilar e Rodar

**Para compilar e rodar imediatamente (Linux/Mac):**

```bash
make run
```

**Ou manualmente via CMake:**

```bash
mkdir build && cd build
cmake ..
make
./RalphArcade
```

---

## 🕶️ Segredos do Hacker (Cheats)

Se você estiver com dificuldades, pode digitar estes códigos na tela de **MENU**:

* `L - I - F - E`: Ativa 99 vidas.
* `G - O - D`: Ativa o Modo Deus (Imortalidade).

---
*Divirta-se e tente bater o recorde mundial!* 🏆🚀
