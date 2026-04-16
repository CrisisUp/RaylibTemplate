import os
import sys

def load_keys(file_path):
    keys = set()
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith('#'):
                key = line.split('=')[0]
                keys.add(key)
    return keys

def main():
    lang_dir = 'resources/lang'
    files = [f for f in os.listdir(lang_dir) if f.endswith('.txt')]
    
    if not files:
        print("Erro: Nenhum arquivo de tradução encontrado.")
        sys.exit(1)
        
    print(f"Validando {len(files)} arquivos de tradução...")
    
    all_keys = {}
    for f in files:
        all_keys[f] = load_keys(os.path.join(lang_dir, f))
        
    # Usar o primeiro arquivo como referência
    ref_file = files[0]
    ref_keys = all_keys[ref_file]
    
    errors = False
    for f in files:
        if f == ref_file:
            continue
            
        current_keys = all_keys[f]
        
        # Chaves faltando no arquivo atual
        missing = ref_keys - current_keys
        if missing:
            print(f"Erro: Chaves faltando em '{f}' (presentes em '{ref_file}'):")
            for k in sorted(missing):
                print(f"  - {k}")
            errors = True
            
        # Chaves extras no arquivo atual (faltando na referência)
        extra = current_keys - ref_keys
        if extra:
            print(f"Erro: Chaves extras em '{f}' (não encontradas em '{ref_file}'):")
            for k in sorted(extra):
                print(f"  - {k}")
            errors = True
            
    if errors:
        print("\nFalha na validação de traduções.")
        sys.exit(1)
    else:
        print("Sucesso: Todos os arquivos de tradução estão sincronizados!")
        sys.exit(0)

if __name__ == "__main__":
    main()
