#include "dungeonBuilder.h"

DungeonBuilder::DungeonBuilder() = default;

DungeonBuilder::~DungeonBuilder() = default;

std::string DungeonBuilder::constructFloor(std::array<int, 2> &t_roomCount, std::array<int, 2> &t_roomWidthRange,
                                           std::array<int, 2> &t_roomHeightRange, std::array<int, 2> &t_tunnelWidthRange,
                                           std::array<int, 2> &t_tunnelHeightRange,
                                           int t_width = 50, int t_height = 50)
{
    // change seed so that each floor is different
    auto time_ui = static_cast<int>(std::time(NULL) % 1000);
    srand(time_ui);

    // check if room count is at least 1
    if (t_roomCount[0] < 1 || t_roomCount[1] < 1)
    {
        return "Room count must be greater than 0";
    }

    // check if floor width & height is valid
    if (t_width * t_height < t_roomCount[1] * ((t_roomWidthRange[0] * t_roomHeightRange[0]) + (t_tunnelWidthRange[1] * t_tunnelHeightRange[1])))
    {
        return "Floor size is too small for room count";
    }

    if (t_roomWidthRange[0] < 2 || t_roomWidthRange[1] < 2 || t_roomHeightRange[0] < 2 || t_roomHeightRange[1] < 2)
    {
        return "Room size is too small";
    }

    if (t_tunnelWidthRange[0] < 2 || t_tunnelWidthRange[1] < 2 || t_tunnelHeightRange[0] < 2 || t_tunnelHeightRange[1] < 2)
    {
        return "Tunnel size is too small";
    }

    // set floor size
    m_floor_width = t_width;
    m_floor_height = t_height;

    // set room count on the floor (it will be random between the two parameters entered)
    m_floor_room_count = rand() % (t_roomCount[1] - t_roomCount[0] + 1) + t_roomCount[0];

    // set room size (if first parameter is greater than second, swap them)
    if (t_roomWidthRange[0] > t_roomWidthRange[1])
    {
        m_room_width_range = {t_roomWidthRange[1], t_roomWidthRange[0]};
    }
    else
    {
        m_room_width_range = t_roomWidthRange;
    }

    // set room size (if first parameter is greater than second, swap them)
    if (t_roomHeightRange[0] > t_roomHeightRange[1])
    {
        m_room_height_range = {t_roomHeightRange[1], t_roomHeightRange[0]};
    }
    else
    {
        m_room_height_range = t_roomHeightRange;
    }

    // set tunnel size (if first parameter is greater than second, swap them)
    if (t_tunnelWidthRange[0] > t_tunnelWidthRange[1])
    {
        m_tunnel_width_range = {t_tunnelWidthRange[1], t_tunnelWidthRange[0]};
    }
    else
    {
        m_tunnel_width_range = t_tunnelWidthRange;
    }

    // set tunnel size (if first parameter is greater than second, swap them)
    if (t_tunnelHeightRange[0] > t_tunnelHeightRange[1])
    {
        m_tunnel_height_range = {t_tunnelHeightRange[1], t_tunnelHeightRange[0]};
    }
    else
    {
        m_tunnel_height_range = t_tunnelHeightRange;
    }

    // create a 2D vector to represent the floor
    pm_floorVector.resize(m_floor_height, std::vector<int>(m_floor_width, TileType::Wall));

    // define needed variables
    int currentNumberOfRooms = 0;
    int iterationCount = 0;

    // create rooms until the room count or iteration count is reached
    while (currentNumberOfRooms <= m_floor_room_count)
    {
        // create the starting room
        int randomRoomWidth = rand() % (m_room_width_range[1] - m_room_width_range[0] + 1) + m_room_width_range[0];
        int randomRoomHeight = rand() % (m_room_height_range[1] - m_room_height_range[0] + 1) + m_room_height_range[0];

        // define room type
        int roomType = RoomType(rand() % 1);

        // start tunnel & room creation
        if (currentNumberOfRooms != 0)
        {
            // create tunnel to the new room, that is created after the tunnel creation
            // number of parts of the tunnel in random between 3 and 6
            int tunnelPartsToAchieve = rand() % 4 + 3;
            int currentTunnelPart = 0;

            // decide if the tunnel will be horizontal or HorizontalLeft
            int tunnelDirection = TunnelType(rand() % 2);

            // the tunnel width is random between the two parameters, but height is 2, when the tunnel's direction is horizontal
            if (tunnelDirection == TunnelType::HorizontalLeft || tunnelDirection == TunnelType::HorizontalRight)
            {
                int randomTunnelWidth = rand() % (m_tunnel_width_range[1] - m_tunnel_width_range[0] + 1) + m_tunnel_width_range[0];
                int randomTunnelHeight = 2;

                // create the tunnel
                // TODO - createTunnel()
            }

            // the tunnel height is random between the two parameters, when the tunnel's direction is vertical
            else if (tunnelDirection == TunnelType::VerticalUp || tunnelDirection == TunnelType::VerticalDown)
            {
                int randomTunnelWidth = 2;
                int randomTunnelHeight = rand() % (m_tunnel_height_range[1] - m_tunnel_height_range[0] + 1) + m_tunnel_height_range[0];

                // create the tunnel
                // TODO - createTunnel()
            }

            // currentNumberOfRooms++;

            // increase iteration count if the operation fails
            iterationCount++;
        }
        else
        {
            int tempRoomX = 0;
            int tempRoomY = 0;
            int tempRoomWidth = 5;
            int tempRoomHeight = 3;

            if (!_create_room(tempRoomX, tempRoomY, tempRoomWidth, tempRoomHeight, roomType))
            {
                return "Failed to create starting room";
            }

            currentNumberOfRooms++;
        }

        if (iterationCount > 1000)
        {
            return "Failed to create floor";
        }
    }

    _printFloor();

    return "Floor created";
}

bool DungeonBuilder::_create_room(const int &t_roomX, const int &t_roomY, int &t_roomWidth, int &t_roomHeight, int const &t_roomType)
{
    // check if the room does not intersect with another object
    if (_object_intersects(t_roomX, t_roomY, t_roomWidth, t_roomHeight))
    {
        return false;
    }

    // create the room
    for (int i = t_roomY; i < t_roomY + t_roomHeight; i++)
    {
        for (int j = t_roomX; j < t_roomX + t_roomWidth; j++)
        {
            pm_floorVector[i][j] = TileType::RoomSpace;
        }
    }

    return true;
}

bool DungeonBuilder::_object_intersects(const int &t_roomX, const int &t_roomY, int &t_roomWidth, int &t_roomHeight)
{
    // if X or Y is out of bounds, return true
    if ((t_roomX + m_room_height_range[0]) > m_floor_height || (t_roomY + m_room_width_range[0]) > m_floor_width)
    {
        std::cout << "Room is out of bounds" << std::endl;
        return true;
    }

    // if object is out of bounds, reduce height
    if (t_roomX + t_roomHeight > m_floor_height)
    {
        t_roomHeight -= (t_roomX + t_roomHeight) - m_floor_height;
    }

    // if object is out of bounds, reduce width
    if (t_roomY + t_roomWidth > m_floor_width)
    {
        t_roomWidth -= (t_roomY + t_roomWidth) - m_floor_width;
    }

    // if object is too small, return true
    if (t_roomWidth < m_room_width_range[0] || t_roomHeight < m_room_height_range[0])
    {
        std::cout << "Room is too small" << std::endl;
        return true;
    }

    // set spacing between objects
    unsigned int spacing = 2;

    // check every tile in the object with the spacing like margin, but check if the tile is not out of bounds
    for (int y = t_roomY - spacing; y < t_roomY + t_roomHeight + spacing; y++)
    {
        for (int x = t_roomX - spacing; x < t_roomX + t_roomWidth + spacing; x++)
        {
            if (!(y >= 0 && x >= 0 && y < m_floor_height && x < m_floor_width))
            {
                continue;
            }

            if (pm_floorVector[y][x] != TileType::Wall)
            {
                return true;
            }
        }
    }

    return false;
}

void DungeonBuilder::_printFloor()
{
    for (int y = 0; y < m_floor_height; y++)
    {
        for (int x = 0; x < m_floor_width; x++)
        {
            if (pm_floorVector[y][x] == TileType::Wall)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << " ";
            }
        }

        std::cout << "\n";
    }
}

int main()
{

    std::array<int, 2> roomCount = {1, 10};
    std::array<int, 2> roomWidthRange = {4, 15};
    std::array<int, 2> roomHeightRange = {2, 6};
    std::array<int, 2> tunnelWidthRange = {5, 10};
    std::array<int, 2> tunnelHeightRange = {5, 10};

    DungeonBuilder dungeonBuilder;
    std::string builderResponse = dungeonBuilder.constructFloor(
        roomCount,         // room count range
        roomWidthRange,    // room width range
        roomHeightRange,   // room height range
        tunnelWidthRange,  // tunnel width range
        tunnelHeightRange, // tunnel height range
        50,                // floor width - default 50
        50                 // floor height - default 50
    );

    std::cout << builderResponse;

    return 0;
}
