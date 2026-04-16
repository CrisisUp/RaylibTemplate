# --- Stage 1: Build Stage ---
FROM emscripten/emsdk:3.1.51 AS build-env

# Instalar dependências básicas de build
RUN apt-get update && apt-get install -y cmake git python3 pip && rm -rf /var/lib/apt/lists/*
RUN pip install Pillow

# Definir diretório de trabalho
WORKDIR /src

# Copiar arquivos do projeto (respeitando o .dockerignore se existir)
COPY . .

# Configurar e compilar o projeto para Web
# Usamos emcmake para que o CMake entenda o ambiente Emscripten
RUN emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build-web

# --- Stage 2: Production Stage (Opcional - Apenas para servir os arquivos) ---
# Se você quiser apenas os arquivos, pode parar no Stage 1. 
# Mas vamos criar um stage que disponibiliza os arquivos via Nginx para teste rápido.
FROM nginx:alpine AS runtime-env

# Copiar os arquivos gerados do build stage para o diretório do Nginx
# Ajuste o nome dos arquivos se o seu projeto gerar nomes diferentes
COPY --from=build-env /src/build-web/RalphArcade.html /usr/share/nginx/html/index.html
COPY --from=build-env /src/build-web/RalphArcade.js /usr/share/nginx/html/
COPY --from=build-env /src/build-web/RalphArcade.wasm /usr/share/nginx/html/
COPY --from=build-env /src/build-web/RalphArcade.data /usr/share/nginx/html/

EXPOSE 80

# O Nginx inicia automaticamente por padrão
