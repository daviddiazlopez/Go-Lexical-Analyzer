#!/bin/bash

echo "🔄 Compilando o proxecto..."
make

# Verificar se a compilación foi exitosa
if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
else
    echo "❌ Erro na compilación."
    exit 1
fi
