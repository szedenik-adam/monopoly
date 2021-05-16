
class FEvent
{
public:
	enum e {
		Nothing = 0,
		Roll = 1,
		End = 2,
		BuyField = 4,
		LeaveJailWithCard = 8,
		LeaveJailWithPaying = 16,
		SellField = 32,
	};
	e ee;

	FEvent(int ee):ee((enum e)ee){}
	FEvent(enum e ee):ee(ee){}
	FEvent():ee(Nothing){}
	
	bool operator==(enum e bee)
	{
		return ee == bee;
	}
	operator bool() {return ee != Nothing;}

};

inline FEvent operator |(FEvent a, FEvent b)
{
	return static_cast<FEvent>(static_cast<int>(a.ee) | static_cast<int>(b.ee));
}

inline FEvent operator &(FEvent a, FEvent b)
{
	return static_cast<FEvent>(static_cast<int>(a.ee) & static_cast<int>(b.ee));
}

inline FEvent operator &(FEvent a, FEvent::e bee)
{
	return static_cast<FEvent>(static_cast<int>(a.ee) & static_cast<int>(bee));
}

inline FEvent& operator |=(FEvent& a, FEvent b)
{
    return a = a |b;
}

inline FEvent& operator |=(FEvent& a, FEvent::e bee)
{
    return a = a | FEvent(bee);
}

inline FEvent& operator &=(FEvent& a, FEvent b)
{
    return a = a & b;
}

inline FEvent& operator &=(FEvent& a, FEvent::e bee)
{
    return a = a & FEvent(bee);
}

