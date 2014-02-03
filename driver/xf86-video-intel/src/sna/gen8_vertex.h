#ifndef GEN8_VERTEX_H
#define GEN8_VERTEX_H

#include "compiler.h"

#include "sna.h"
#include "sna_render.h"

void gen8_vertex_align(struct sna *sna, const struct sna_composite_op *op);
void gen8_vertex_flush(struct sna *sna);
int gen8_vertex_finish(struct sna *sna);
void gen8_vertex_close(struct sna *sna);

#endif /* GEN8_VERTEX_H */
