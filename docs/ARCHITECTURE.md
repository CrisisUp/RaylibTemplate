# Arquitetura do RalphArcade

Este documento descreve a estrutura técnica do projeto e como estendê-lo.

## 1. Visão Geral
O projeto utiliza uma arquitetura baseada em **Estados (Scenes)** e **Managers (Singletons)**.

### Managers Principais
- **ResourceManager**: Centraliza o carregamento de texturas e sons para evitar duplicatas na memória.
- **InputManager**: Abstrai as entradas do teclado/gamepad.
- **LocalizationManager**: Gerencia as traduções (`en.txt`, `pt.txt`).
- **ConfigManager**: Lê e salva configurações no `config.txt`.

## 2. Como adicionar uma nova Cena (Scene)
1. Crie uma classe que herde de `Scene` (veja `include/Scene.h`).
2. Implemente os métodos `Update()` e `Draw()`.
3. Para mudar para sua nova cena, use `game->ChangeScene(std::make_unique<SuaCena>(game))`.

## 3. Como adicionar um novo Inimigo
1. O sistema de inimigos baseia-se na classe `Enemy`.
2. Para novos comportamentos, você pode herdar de `Enemy` ou modificar o `Enemy::Update` para lidar com diferentes tipos baseados em um `enum`.

## 4. Documentação de Código
Usamos o padrão **Doxygen**. Para documentar uma função, use:
```cpp
/**
 * @brief Explicação curta da função.
 * @param param1 Descrição do parâmetro.
 * @return Descrição do retorno.
 */
void MinhaFuncao(int param1);
```
