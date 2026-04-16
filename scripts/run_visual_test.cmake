# Script auxiliar para rodar o teste visual sequencialmente
set(EXE_NAME "./RalphArcade")
if(APPLE)
    set(EXE_NAME "./RalphArcade.app/Contents/MacOS/RalphArcade")
elseif(WIN32)
    set(EXE_NAME "./RalphArcade.exe")
endif()

# 1. Roda o jogo para capturar o screenshot
message("Executando jogo para captura visual: ${EXE_NAME}")
execute_process(
    COMMAND ${EXE_NAME} --screenshot-test 60 --output test_capture.png
    RESULT_VARIABLE game_result
)

if(NOT game_result EQUAL 0)
    message(FATAL_ERROR "O jogo crashou ou falhou durante a captura visual!")
endif()

# 2. Compara com a referência usando o script Python
message("Comparando captura com referência...")
execute_process(
    COMMAND python3 ../scripts/verify_visuals.py ../tests/visual/menu_golden.png test_capture.png
    RESULT_VARIABLE comp_result
)

if(NOT comp_result EQUAL 0)
    message(FATAL_ERROR "Regressão visual detectada!")
endif()

message("Teste visual concluído com sucesso!")
