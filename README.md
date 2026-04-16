# Ralph Arcade - Legendary Repaired Edition

Bem-vindo ao **Ralph Arcade**, um jogo de ação frenética em estilo retro construído com a biblioteca Raylib. Você controla o Ralph e deve sobreviver em mundos gerados proceduralmente enquanto busca a pontuação máxima!

## 🕹️ Como Jogar

O objetivo é simples: **colete todas as moedas amarelas** para avançar de nível. Mas cuidado, os inimigos e o cenário ficarão mais perigosos a cada passo!

### Controles

* **Setas do Teclado:** Movimentam o Ralph.
* **Barra de Espaço:** Ativa o **DASH** (Impulso rápido). Possui um tempo de recarga de 0.6s.
* **Enter:** Inicia o jogo ou avança entre telas (Menu, Level Up, Game Over).

---

## 🚀 Mecânicas Principais

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

## 🖥️ Requisitos e Compilação (Mac)

Certifique-se de ter a Raylib instalada (`brew install raylib`).

**Para compilar e rodar imediatamente:**

```bash
make run
```

---

## 🕶️ Segredos do Hacker (Cheats)

Se você estiver com dificuldades, pode digitar estes códigos na tela de **MENU**:

* `L - I - F - E`: Ativa 99 vidas.
* `G - O - D`: Ativa o Modo Deus (Imortalidade).

---
*Divirta-se e tente bater o recorde mundial!* 🏆🚀
