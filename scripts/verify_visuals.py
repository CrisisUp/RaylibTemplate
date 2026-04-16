import sys
import os

def compare_images_binary(img1_path, img2_path):
    """
    Compara duas imagens bit a bit. 
    Para um teste de regressão visual estável (mesmo hardware), 
    isso deve ser idêntico.
    """
    if not os.path.exists(img1_path):
        print(f"Erro: Imagem de referência não encontrada: {img1_path}")
        return False
    if not os.path.exists(img2_path):
        print(f"Erro: Imagem de teste não encontrada: {img2_path}")
        return False

    with open(img1_path, 'rb') as f1, open(img2_path, 'rb') as f2:
        if f1.read() == f2.read():
            return True
    
    return False

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python verify_visuals.py reference.png capture.png")
        sys.exit(1)
    
    ref = sys.argv[1]
    cap = sys.argv[2]
    
    if compare_images_binary(ref, cap):
        print("✅ Teste Visual PASSOU (Identidade Binária)!")
        sys.exit(0)
    else:
        print("❌ Teste Visual FALHOU (Arquivos Diferentes)!")
        print("Dica: Se a mudança foi intencional, atualize a imagem de referência em tests/visual/")
        sys.exit(1)
