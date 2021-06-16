#ifndef DISPLAYDATA_HPP
#define DISPLAYDATA_HPP

struct SLastCession;

struct SDisplayData
{
public:
  void save (SLastCession& lastCession) const;
  void restore (SLastCession& lastCession);

  int m_opacity = 70;
  bool m_circle = false;
};

#endif // DISPLAYDATA_HPP
