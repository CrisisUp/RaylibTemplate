import os
import re
import sys

def check_localization_consistency():
    """Verifica se en.txt e pt.txt têm as mesmas chaves."""
    print("--- Verificando Consistência de Localização ---")
    langs_dir = "resources/lang"
    en_path = os.path.join(langs_dir, "en.txt")
    pt_path = os.path.join(langs_dir, "pt.txt")
    
    if not os.path.exists(en_path) or not os.path.exists(pt_path):
        print("Erro: en.txt ou pt.txt não encontrado!")
        return False
        
    def get_keys(path):
        keys = set()
        with open(path, 'r', encoding='utf-8') as f:
            for line in f:
                if '=' in line and not line.startswith('#'):
                    keys.add(line.split('=')[0].strip())
        return keys

    en_keys = get_keys(en_path)
    pt_keys = get_keys(pt_path)
    
    missing_in_pt = en_keys - pt_keys
    missing_in_en = pt_keys - en_keys
    
    if missing_in_pt:
        print(f"Aviso: Chaves presentes em en.txt mas faltando em pt.txt: {missing_in_pt}")
    if missing_in_en:
        print(f"Aviso: Chaves presentes em pt.txt mas faltando em en.txt: {missing_in_en}")
        
    if not missing_in_pt and not missing_in_en:
        print("Sucesso: Localização consistente.")
        return True
    return False

def check_orphaned_assets():
    """Verifica se arquivos de resources/ estão sendo usados ou se faltam arquivos."""
    print("\n--- Verificando Integridade de Assets ---")
    resources_dir = "resources"
    assets_referenced = set()
    
    # Vamos procurar por referências no código C++ e include/
    for root_dir in ["src", "include"]:
        if os.path.exists(root_dir):
            for root, dirs, files in os.walk(root_dir):
                for file in files:
                    if file.endswith(('.cpp', '.h')):
                        with open(os.path.join(root, file), 'r', errors='ignore') as f:
                            content = f.read()
                            # Procura por strings que pareçam caminhos de resources
                            matches = re.findall(r'resources/[^"\', \n)]+', content)
                            for match in matches:
                                assets_referenced.add(match.strip())

    missing = []
    for asset in assets_referenced:
        if not os.path.exists(asset):
            missing.append(asset)

    if missing:
        print(f"Erro: Assets referenciados no código mas não encontrados no disco:")
        for m in missing:
            print(f"  - {m}")
        return False
    else:
        print(f"Sucesso: Todos os {len(assets_referenced)} assets referenciados existem no disco.")
        return True

if __name__ == "__main__":
    # Garante que o script rode da raiz do projeto
    if not os.path.exists("resources"):
        print("Erro: Execute este script da raiz do projeto.")
        sys.exit(1)
        
    loc_ok = check_localization_consistency()
    assets_ok = check_orphaned_assets()
    
    if not loc_ok or not assets_ok:
        sys.exit(1)
    sys.exit(0)
