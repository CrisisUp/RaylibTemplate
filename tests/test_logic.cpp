#include <gtest/gtest.h>
#include "ConfigManager.h"
#include <fstream>
#include <cstdio>

// Exemplo de teste para o ConfigManager
TEST(ConfigManagerTest, LoadDefaultValues) {
    // Cria um arquivo temporário de config para o teste
    std::ofstream testConfig("test_config.txt");
    testConfig << "language=pt\n";
    testConfig << "volume=80\n";
    testConfig.close();

    ConfigManager::Instance().Load("test_config.txt");
    
    EXPECT_EQ(ConfigManager::Instance().GetString("language", "en"), "pt");
    EXPECT_EQ(ConfigManager::Instance().GetString("volume", "0"), "80");
    EXPECT_EQ(ConfigManager::Instance().GetString("non_existent", "default"), "default");

    std::remove("test_config.txt");
}

// Exemplo de teste matemático simples (poderia ser lógica de colisão)
TEST(MathLogicTest, SimpleCollisionCalculation) {
    float playerX = 100.0f;
    float enemyX = 105.0f;
    float distance = std::abs(playerX - enemyX);
    
    EXPECT_LT(distance, 10.0f); // Se a distância for menor que 10, há colisão
}
