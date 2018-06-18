#include "World.h"

#include <iostream>

World::World() {

}

const sf::Vector2f World::getCenter() const{
    int index{0};
    float xPos{0};
    float yPos{0};

    if(m_chunks.size() == 0){
        return {0,0};
    }
    else if(m_chunks.size() % 2 == 0){
        index = m_chunks.size() / 2;
        xPos = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
        yPos = 300;
    }
    else{
        index = m_chunks.size() / 2;
        xPos = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
        xPos += (CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X) / 2;
        yPos = 300;
    }

    return {xPos,yPos};
}

void World::addChunk(int _num) {
    for(int i{0}; i < _num; i++) {
        m_chunks.push_back(m_chunks.size());
    }
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

    currentNumber.clear();

    if(chunkIDs.empty()) {
        return;
    }

    auto it = chunkIDs.begin();
    int currentChunkID{*it};


    for(char& c : _data.invisibleBlocks) {
        if(c == '.') {
            int id = std::stoi(currentNumber);
            currentNumber.clear();
            m_chunks[currentChunkID].toggleVisibility(id, false);
        }

        else if(c == '%') {
            if(it != chunkIDs.end() - 1) {
                it++;
                currentChunkID = *it;
            }
        }

        else {
            currentNumber += c;
        }
    }
}
