# Changelog

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

O formato é baseado em [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
e este projeto adere ao [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2026-04-15

### Adicionado
- **Sistema de Pausa:** Pressione a tecla `P` durante o gameplay para pausar.
- **Interface de Pausa:** Overlay visual com a mensagem "JOGO PAUSADO" (localizado).
- **Localização:** Adicionada chave `MSG_PAUSED` nos arquivos `en.txt` e `pt.txt`.
- **Manual de Contribuição:** Novo arquivo `CONTRIBUTING.md` com diretrizes para desenvolvedores.

### Corrigido
- **Áudio:** Corrigida falha onde a música de fundo não tocava (falta de `UpdateMusicStream`).
- **Progressão de Nível:** Corrigido bug onde o nível ficava travado no 1 (variável não era incrementada).
- **Persistência:** Pontuação e nível agora são mantidos corretamente ao transitar entre fases.
- **Velocidade dos Inimigos:** Inimigo tipo `HUNTER` agora respeita a velocidade definida no `config.txt`.

### Alterado
- **Balanceamento de Velocidade:** 
  - Velocidade base do jogador reduzida para 200 (mais controle).
  - Velocidade dos inimigos reduzida significativamente no início do jogo.
  - Incremento de dificuldade por nível reduzido de 2% para 1% para uma curva mais suave.
  - Movimentação diagonal dos inimigos `BOUNCER` agora é normalizada (não acelerada).
- **Organização:** Inicialização de variáveis globais de estado movida para o construtor da classe `Game`.

## [0.1.0] - 2026-04-10
- Versão inicial do protótipo Ralph Arcade.
