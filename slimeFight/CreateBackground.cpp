#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include"map.h"


int createBackground(VertexArray& rVA, map& arena)
{
	// Anything we do to rVA we are actually doing to background (in the main function)

	// How big is each tile/texture
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 4;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width ;
	int worldHeight = arena.height ;

	// What type of primitive are we using?
	rVA.setPrimitiveType(Quads);

	// Set the size of the vertex arra
	rVA.resize(worldWidth*worldHeight * VERTS_IN_QUAD);

	// Start at the beginning of the vertex array
	int currentVertex = 0;

	for ( int h = 0; h < worldHeight; h++)
	{
		for (int w = 0; w < worldWidth; w++)
		{
			
				rVA[currentVertex + 0].position = Vector2f(arena.space[h][w]->X, arena.space[h][w]->Y);
				rVA[currentVertex + 1].position = Vector2f(arena.space[h][w]->X + TILE_SIZE, arena.space[h][w]->Y);
				rVA[currentVertex + 2].position = Vector2f(arena.space[h][w]->X + TILE_SIZE, arena.space[h][w]->Y + TILE_SIZE);
				rVA[currentVertex + 3].position = Vector2f(arena.space[h][w]->X, arena.space[h][w]->Y + TILE_SIZE);
				//rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
				//rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
				//rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
				//rVA[currentVertex + 3].position = Vector2f((w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE);
				// Define the position in the Texture to draw for current quad
				// Either mud, stone, grass or wall
				if (arena.space[h][w]->ifCollusible)
				{
					// Use the wall texture

					int mOrG = (rand() % TILE_TYPES-1);
					int verticalOffset = mOrG * TILE_SIZE;
					rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset + TILE_SIZE);
					rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset + TILE_SIZE);
					rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset + TILE_SIZE);
					rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset + TILE_SIZE);

					
				}
				else if (arena.space[h][w]->texturetype == usoftexture::wall)
				{
					rVA[currentVertex + 0].texCoords = Vector2f(0, 0 );
					rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 );
					rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE );
					rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE );
				}
				else
				{
					// Use a random floor texture
					
					int mOrG = (rand() % TILE_TYPES);
					int verticalOffset = mOrG * TILE_SIZE;
					rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE+verticalOffset);
					rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE+verticalOffset);
					rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE+verticalOffset);
					rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE+verticalOffset);

					

				}

				// Position ready for the next for vertices
				currentVertex = currentVertex + VERTS_IN_QUAD;
			
		}
	}

	return TILE_SIZE;
}