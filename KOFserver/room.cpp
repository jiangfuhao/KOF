#include "room.h"
#include "udpserver.h"
#include "player.h"
#include <iostream>

using std::cout;        using std::endl;

Room::Room(int id, std::weak_ptr<Player> roomOwner)
	: m_id{id}, m_currentRoomMemberNumber{1}, _roomOwner{roomOwner}
{}

bool Room::amITheRoomOwner(std::weak_ptr<Player> isRoomOwner)
{
    return isRoomOwner.lock() == _roomOwner.lock();
}

bool Room::isFull()
{
    return m_currentRoomMemberNumber == MAX_ROOM_MEMBER_NUMBER;
}

int Room::id() const
{
	return m_id;
}

bool Room::joinMember(std::weak_ptr<Player> member)
{
    if(!isFull())
    {
        _members = member;
		++m_currentRoomMemberNumber;
        return true;
    }
	cout << "Room " << m_id << " is full, can not join" << endl;
    return false;
}

void Room::memberExit(std::weak_ptr<Player> member)
{
    --m_currentRoomMemberNumber;
    if(!(member.lock() == _roomOwner.lock() ||
            member.lock() == _members.lock()))
        cout << "The player is not a member of the room" << endl;
}

void Room::relayMessagesToMembers(std::weak_ptr<Player> sender, transimissionMessage &message)
{
	if(sender.lock() == _roomOwner.lock() && !_members.expired())
        _members.lock()->sendToMe(message);
	else if(sender.lock() == _members.lock() && !_roomOwner.expired())
        _roomOwner.lock()->sendToMe(message);
}