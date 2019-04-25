#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "WorldChunk.h"

typedef std::vector<WorldChunk::EncodedChunkData> encodedChunks;
struct WorldTime;

class World {
	public:
		////////////////////////////////////////////////
		//TODO: maybe concat m_hours and m_minutes into one
		//so it's like 2344 instead of 23 and 44?

		struct WorldTime {

			WorldTime(int _hours, int _minutes) {
				setTime(_hours, _minutes);
			}

			void setTime(int _hours, int _minutes) {
				hours = _hours;
				minutes = _minutes;

				if(minutes >= 60) {
					minutes = 0;
					hours++;
				}

				if(hours >= 24){
					hours = 0;
				}
			}

			//increment by one minute
			void tick() {
				setTime(hours, minutes + 1);
			}

			const std::string getString() const {
				std::string minutesStr{std::to_string(minutes)};
				if(minutes < 10) {
					minutesStr = "0" + minutesStr;
				}
				return std::to_string(hours) + ":" + minutesStr;
			}


			int hours;
			int minutes;
		};

		////////////////////////////////////////////////

		World();

		const sf::Vector2f getCenter() const;
		void addChunk(int _beginID, int _num, bool _empty);
		const std::vector<WorldChunk>& getChunks() const;

		const encodedChunks encodeChunks() const;
		void parseChunk(const WorldChunk::EncodedChunkData& _data);

		void update(int _timeslice);

		const WorldTime getTime() const;
	private:
		std::vector<WorldChunk> m_chunks;
		WorldTime m_time;
};

#endif // WORLD_H_INCLUDED
