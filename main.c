#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "Map.h"
#include "list.h"

/* =======================
   MODELOS DE DATOS
   ======================= */

typedef struct {
    char *texto;
    char *area;
} Pregunta;

typedef struct {
    char *nombre;
    char *area;
} Carrera;

/* =======================
   UTILIDADES
   ======================= */

static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *copy = malloc(strlen(s) + 1);
    if (!copy) return NULL;
    strcpy(copy, s);
    return copy;
}

void trim(char *s) {
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return;

    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

char *csv_field(const char *line, int index) {
    char *tmp = strdup_safe(line);
    char *tok = strtok(tmp, ",\n");

    while (tok && --index)
        tok = strtok(NULL, ",\n");

    char *out = tok ? strdup_safe(tok) : NULL;
    free(tmp);
    return out;
}

/* =======================
   HASH MAP
   ======================= */

long long stringHash(const void *key) {
    long long hash = 5381;
    const char *str = key;
    while (*str)
        hash = ((hash << 5) + hash) + tolower(*str++);
    return hash;
}

int stringEqual(const void *a, const void *b) {
    return strcmp(a, b) == 0;
}

/* =======================
   CARGA DE DATOS
   ======================= */

void cargar_preguntas(const char *archivo, list *destino) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("Error abriendo preguntas");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        Pregunta *p = malloc(sizeof(Pregunta));
        p->texto = csv_field(buffer, 1);
        p->area  = csv_field(buffer, 2);
        trim(p->area);
        list_push_back(destino, p);
    }
    fclose(f);
}

void cargar_carreras(const char *archivo, Map *map) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("Error abriendo carreras");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        Carrera *c = malloc(sizeof(Carrera));
        c->nombre = csv_field(buffer, 1);
        c->area   = csv_field(buffer, 2);
        trim(c->area);

        list *l = searchMap(map, c->area);
        if (!l) {
            l = list_create_empty();
            insertMap(map, c->area, l);
        }
        list_push_back(l, c);
    }
    fclose(f);
}

/* =======================
   MOTOR DE SCORING
   ======================= */

void ejecutar_test(list *preguntas,
                   int *ing, int *sal,
                   int *art, int *cs) {

    char respuesta[8];

    for (Pregunta *p = list_first(preguntas); p; p = list_next(preguntas)) {
        printf("%s (si/no): ", p->texto);
        scanf("%7s", respuesta);

        if (strcmp(respuesta, "si") == 0) {
            if (!strcmp(p->area, "ingenieria")) (*ing)++;
            else if (!strcmp(p->area, "salud")) (*sal)++;
            else if (!strcmp(p->area, "artes")) (*art)++;
            else if (!strcmp(p->area, "cienciaSociales")) (*cs)++;
        }
    }
}

/* =======================
   PRESENTACIÓN
   ======================= */

void mostrar_resultados(Map *map, const char *area) {
    list *carreras = searchMap(map, area);

    if (!carreras) {
        printf("[ERROR] No se encontraron carreras para el área '%s'\n", area);
        return;
    }

    printf("\nCarreras recomendadas (%s):\n", area);
    for (Carrera *c = list_first(carreras); c; c = list_next(carreras)) {
        printf(" - %s\n", c->nombre);
    }
}

/* =======================
   EXPORTAR RESULTADO
   ======================= */

void exportar_resultado(const char *archivo,
                        int ing, int sal, int art, int cs,
                        const char *area_final,
                        Map *map) {

    FILE *f = fopen(archivo, "w");
    if (!f) {
        perror("No se pudo crear archivo de salida");
        return;
    }

    fprintf(f, "RESULTADO TEST VOCACIONAL\n");
    fprintf(f, "========================\n\n");

    fprintf(f, "Puntajes:\n");
    fprintf(f, "Ingeniería: %d\n", ing);
    fprintf(f, "Salud: %d\n", sal);
    fprintf(f, "Artes: %d\n", art);
    fprintf(f, "Ciencias Sociales: %d\n\n", cs);

    fprintf(f, "Área recomendada: %s\n\n", area_final);
    fprintf(f, "Carreras sugeridas:\n");

    list *carreras = searchMap(map, area_final);
    if (carreras) {
        for (Carrera *c = list_first(carreras); c; c = list_next(carreras)) {
            fprintf(f, "- %s\n", c->nombre);
        }
    } else {
        fprintf(f, "No se encontraron carreras.\n");
    }

    fclose(f);
}

/* =======================
   MAIN
   ======================= */

int main(void) {
    list *preguntas = list_create_empty();
    Map *map = createMap(stringHash, stringEqual);

    cargar_preguntas("preguntas_test_vocacional.csv", preguntas);
    cargar_carreras("carreras.csv", map);

    int ing = 0, sal = 0, art = 0, cs = 0;

    ejecutar_test(preguntas, &ing, &sal, &art, &cs);

    printf("\nResultados:\n");
    printf("Ingeniería: %d\n", ing);
    printf("Salud: %d\n", sal);
    printf("Artes: %d\n", art);
    printf("Ciencias Sociales: %d\n", cs);

    const char *area_final = "ingenieria";
    int max = ing;

    if (sal > max) { max = sal; area_final = "salud"; }
    if (art > max) { max = art; area_final = "artes"; }
    if (cs  > max) { max = cs;  area_final = "cienciaSociales"; }

    printf("\nÁrea recomendada: %s\n", area_final);
    mostrar_resultados(map, area_final);

    exportar_resultado("resultado.txt",
                       ing, sal, art, cs,
                       area_final,
                       map);

    printf("\nResultado exportado a 'resultado.txt'\n");

    return 0;
}
