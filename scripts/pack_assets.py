import os
import struct
import sys

def pack_assets(input_dir, output_file):
    print(f"--- Empacotando Assets em {output_file} ---")
    
    if not os.path.exists(input_dir):
        print(f"Erro: Diretorio {input_dir} não encontrado.")
        return False

    files_to_pack = []
    for root, dirs, files in os.walk(input_dir):
        for file in files:
            if file.startswith('.') or file.endswith('.py'):
                continue
            full_path = os.path.join(root, file)
            # Normaliza o caminho para usar barras normais (cross-platform)
            rel_path = full_path.replace(os.sep, '/')
            files_to_pack.append((rel_path, full_path))

    # Formato do Arquivo:
    # 4 bytes: Magico "RPAK"
    # 4 bytes: Versão (int 1)
    # 4 bytes: Numero de arquivos
    # Tabela: [64 bytes nome][4 bytes offset][4 bytes size] * Numero de arquivos
    # Dados Binarios...

    MAGIC = b"RPAK"
    VERSION = 1
    num_files = len(files_to_pack)
    
    # Tamanho de uma entrada no indice: 64 + 4 + 4 = 72 bytes
    header_size = 4 + 4 + 4
    index_size = num_files * 72
    current_offset = header_size + index_size

    packed_data = bytearray()
    index_entries = []

    for rel_path, full_path in files_to_pack:
        with open(full_path, "rb") as f:
            data = f.read()
            size = len(data)
            
            # Corta o nome se for maior que 63 caracteres (reserva 1 para null terminator)
            name_encoded = rel_path.encode('utf-8')[:63].ljust(64, b'\0')
            
            index_entries.append(struct.pack("<64sII", name_encoded, current_offset, size))
            packed_data.extend(data)
            current_offset += size

    with open(output_file, "wb") as f:
        f.write(MAGIC)
        f.write(struct.pack("<I", VERSION))
        f.write(struct.pack("<I", num_files))
        for entry in index_entries:
            f.write(entry)
        f.write(packed_data)

    print(f"Sucesso: {num_files} arquivos empacotados ({len(packed_data) / 1024:.2f} KB).")
    return True

if __name__ == "__main__":
    pack_assets("resources", "resources.pak")
