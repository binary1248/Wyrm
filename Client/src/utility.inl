template <typename T>
inline std::string string_cast( T value ) {
	static std::stringstream sstr;

	sstr.str("");

	sstr << value;

	return sstr.str();
}

inline float clean_angle( float angle ) {
	return ( ( angle + 90 ) / 180 ) * M_PI;
}
