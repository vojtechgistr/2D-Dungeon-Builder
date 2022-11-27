#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <array>

class DungeonBuilder
{
public:
    enum TileType
    {
        Wall,
        Space,
        RoomSpace
    };

    enum TunnelType
    {
        HorizontalLeft,
        HorizontalRight,
        VerticalUp,
        VerticalDown
    };

    enum RoomType
    {
        Normal
    };

    struct roomStruct
    {
        int x;
        int y;
        int roomWidth;
        int roomHeight;
        int roomType = Normal;
    };

    DungeonBuilder();
    ~DungeonBuilder();

    std::string constructFloor(std::array<int, 2> &t_roomCount, std::array<int, 2> &t_roomWidthRange,
                               std::array<int, 2> &t_roomHeightRange, std::array<int, 2> &t_tunnelWidthRange,
                               std::array<int, 2> &t_tunnelHeightRange,
                               int t_width, int t_height);

private:
    int m_floor_width;
    int m_floor_height;
    int m_floor_room_count;

    std::array<int, 2> m_room_width_range;
    std::array<int, 2> m_room_height_range;
    std::array<int, 2> m_tunnel_width_range;
    std::array<int, 2> m_tunnel_height_range;

    std::vector<std::vector<int>> pm_floorVector;
    std::vector<roomStruct> pm_roomVector;

    bool _create_room(const int &t_roomX, const int &t_roomY, int &t_roomWidth, int &t_roomHeight, int const &t_roomType);
    bool _object_intersects(const int &t_roomX, const int &t_roomY, int &t_roomWidth, int &t_roomHeight);                 // TODO - check if the object intersects with another object
    bool _create_tunnel(int &t_tunnelX, int &t_tunnelY, int &t_tunnelWidth, int &t_tunnelHeight, int &t_tunnelDirection); // TODO - create a parted tunnel

    void _printFloor();
};

#endif
