#include "World.h"
#include <iostream>

World::World() {
    m_chunks.push_back(0);
    m_chunks.push_back(1);
    m_chunks.push_back(2);
    m_chunks.push_back(3);
}

const std::vector<WorldChunk>& World::getChunks() const {
    return m_chunks;
}

const World::EncodedWorldData World::encodeData() const {
    /*
    chunkIDs will contain the IDs of each chunk, delimited
    by % (ex: 0%1%2%3%4)

    invisibleBlocks will contain the position blocks in the chunk
    that are invisible, with the positions delimited by . and the
    corresponding chunks delimited by %

    For example:

    14.156.332.745.1020%599.766.923.1004%...

    all the blocks before the first % symbol would correspond to the
    chunk with the ID 0, the blocks after the symbol to the chunk with
    ID 1, and so on.
    */

    EncodedWorldData data;

    for(int i{0}; i < m_chunks.size(); i++) {
        data.chunkIDs += std::to_string(m_chunks[i].getID());
        data.chunkIDs += "%";

        auto blocks = m_chunks[i].getBlocks();

        for(int z{0}; z < blocks.size(); z++) {

            if(!blocks[z].getVisibility()) {
                data.invisibleBlocks += std::to_string(z);
                data.invisibleBlocks += ".";
            }
        }


        data.invisibleBlocks += "%";

    }

    //std::cout << "chunkIDs: " << data.chunkIDs << std::endl;
    //std::cout << "invisibleBlocks: " << data.invisibleBlocks << std::endl;

    return data;
}

void World::parseData(World::EncodedWorldData& _data) {
    m_chunks.clear();

    std::vector<int> chunkIDs;

    std::string currentNumber;

    for(char& c : _data.chunkIDs) {
        if(c != '%') {
            currentNumber += c;
        }
        else {
            int id = std::stoi(currentNumber);
            currentNumber.clear();
            //std::cout << "adding index " << id << std::endl;
            m_chunks.push_back(id);
            chunkIDs.push_back(id);
        }
    }

    std::cout << std::endl;
    currentNumber.clear();

    auto it = chunkIDs.begin();
    int currentChunkID{*it};


    for(char& c : _data.invisibleBlocks) {
		if(c == '.') {
            int id = std::stoi(currentNumber);
            currentNumber.clear();
            m_chunks[currentChunkID].toggleVisibility(id, false);
        }

		else if(c == '%') {
			if(it != chunkIDs.end() - 1){
				it++;
				currentChunkID = *it;
			}
		}

        else {
            currentNumber += c;
        }




    }


}
