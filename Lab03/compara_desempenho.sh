#!/bin/bash

# Script de comparacao de desempenho para o Laboratorio 3
# Argumentos: <executavel_sequencial> <executavel_paralelo> <tamanho_array> <num_threads>

if [ "$#" -ne 4 ]; then
    echo "Uso: $0 <executavel_sequencial> <executavel_paralelo> <tamanho_array> <num_threads_paralelo>"
    exit 1
fi

EXEC_SEQ=$1
EXEC_PAR=$2
TAMANHO=$3
THREADS=$4

echo "------------------------------------------------"
echo "Executando versao SEQUENCIAL com $TAMANHO elementos..."
echo "------------------------------------------------"
OUT_SEQ=$($EXEC_SEQ $TAMANHO)
echo "$OUT_SEQ"

echo ""
echo "------------------------------------------------"
echo "Executando versao PARALELA ($THREADS threads) com $TAMANHO elementos..."
echo "------------------------------------------------"
OUT_PAR=$($EXEC_PAR $TAMANHO $THREADS)
echo "$OUT_PAR"

echo ""
echo "------------------------------------------------"
echo "Resultados de Desempenho:"
echo "------------------------------------------------"
# Extrai os tempos usando grep e awk
TEMPO_SEQ=$(echo "$OUT_SEQ" | grep -i "Tempo Sequencial:" | awk '{print $3}')
TEMPO_PAR=$(echo "$OUT_PAR" | grep -i "Tempo Paralelo:" | awk '{print $3}')

if [ -n "$TEMPO_SEQ" ] && [ -n "$TEMPO_PAR" ]; then
    # Calcula o speedup com awk para suportar ponto flutuante sem precisar de bc
    SPEEDUP=$(awk "BEGIN {printf \"%.2fx\", $TEMPO_SEQ / $TEMPO_PAR}")
    echo "Speedup (Tempo Sequencial / Tempo Paralelo): $SPEEDUP"
else
    echo "Nao foi possivel calcular o speedup."
    echo "Certifique-se de que seus programas imprimam 'Tempo Sequencial: [valor] ms' e 'Tempo Paralelo: [valor] ms'."
fi
echo "------------------------------------------------"
echo "Fim da comparacao."
