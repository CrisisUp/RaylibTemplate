import sys
import subprocess
import re

def run_benchmark(exe_path, frames=300, min_fps=60.0):
    print(f"Rodando benchmark: {exe_path} por {frames} frames...")
    
    try:
        # Executa o jogo em modo benchmark
        result = subprocess.run(
            [exe_path, "--benchmark", str(frames)],
            capture_output=True,
            text=True,
            timeout=60
        )
        
        output = result.stdout
        print(output)

        # Procura pelo FPS médio usando Regex
        match = re.search(r"Avg FPS: ([\d.]+)", output)
        if match:
            avg_fps = float(match.group(1))
            print(f"FPS Médio Detectado: {avg_fps:.2f}")
            
            if avg_fps >= min_fps:
                print(f"✅ PERFORMANCE OK! (Min: {min_fps})")
                return True
            else:
                print(f"❌ PERFORMANCE BAIXA! (Min: {min_fps})")
                return False
        else:
            print("❌ Erro: Não foi possível encontrar os resultados do benchmark na saída.")
            return False

    except subprocess.TimeoutExpired:
        print("❌ Erro: O benchmark travou (Timeout).")
        return False
    except Exception as e:
        print(f"❌ Erro ao rodar benchmark: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python verify_performance.py path/to/executable [min_fps] [frames]")
        sys.exit(1)
    
    exe = sys.argv[1]
    min_fps = float(sys.argv[2]) if len(sys.argv) > 2 else 60.0
    frames = int(sys.argv[3]) if len(sys.argv) > 3 else 300
    
    if run_benchmark(exe, frames, min_fps):
        sys.exit(0)
    else:
        sys.exit(1)
