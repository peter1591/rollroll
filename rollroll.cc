#include <array>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

static double values[6][6][6][6][6];

double calculate_value(std::array<int, 5> dices, int all_bingo_counter, int all_bingo_total)
{
  // count[0]: how many `dices` are 0
  int count[6]{0};
  for (int v = 0; v < 6; ++v)
  {
    for (int dice : dices)
    {
      if (dice == v)
      {
        count[v]++;
      }
    }
  }

  const int points_to_all_bingo = all_bingo_total - all_bingo_counter;

  // 11111
  for (int v = 0; v < 6; ++v)
  {
    if (count[v] == 5)
    {
      return (double)points_to_all_bingo / 3.5 * (v + 1);
    }
  }

  // 1111x
  for (int v = 0; v < 6; ++v)
  {
    if (count[v] == 4)
    {
      return (v + 1) * 4;
    }
  }

  // 12345
  if (count[0] > 0 && count[1] > 0 && count[2] > 0 && count[3] > 0 &&
      count[4] > 0)
  {
    return (1 + 2 + 3 + 4 + 5);
  }
  if (count[1] > 0 && count[2] > 0 && count[3] > 0 && count[4] > 0 &&
      count[5] > 0)
  {
    return (2 + 3 + 4 + 5 + 6);
  }

  // 11122
  for (int v1 = 0; v1 < 6; ++v1)
  {
    for (int v2 = 0; v2 < 6; ++v2)
    {
      if (v1 == v2)
        continue;
      if (count[v1] == 3 && count[v2] == 2)
      {
        return (v1 + 1) * 3 + (v2 + 1) * 2;
      }
    }
  }

  // 1234
  if (count[0] > 0 && count[1] > 0 && count[2] > 0 && count[3] > 0)
  {
    return (1 + 2 + 3 + 4);
  }
  if (count[1] > 0 && count[2] > 0 && count[3] > 0 && count[4] > 0)
  {
    return (2 + 3 + 4 + 5);
  }
  if (count[2] > 0 && count[3] > 0 && count[4] > 0 && count[5] > 0)
  {
    return (3 + 4 + 5 + 6);
  }

  // 111
  for (int v1 = 0; v1 < 6; ++v1)
  {
    if (count[v1] == 3)
    {
      return (v1 + 1) * 3;
    }
  }

  // 1122
  for (int v1 = 0; v1 < 6; ++v1)
  {
    for (int v2 = 0; v2 < 6; ++v2)
    {
      if (v1 == v2)
        continue;
      if (count[v1] == 2 && count[v2] == 2)
      {
        return (v1 + 1) * 2 + (v2 + 1) * 2;
      }
    }
  }

  // 11
  for (int v1 = 0; v1 < 6; ++v1)
  {
    if (count[v1] == 2)
    {
      return (v1 + 1) * 2;
    }
  }

  return 1;
}

double expectedValueGiven(int a)
{
  double total = 0;
  int count = 0;
  for (int b = 0; b < 6; ++b)
  {
    for (int c = 0; c < 6; ++c)
    {
      for (int d = 0; d < 6; ++d)
      {
        for (int e = 0; e < 6; ++e)
        {
          ++count;
          total += values[a][b][c][d][e];
        }
      }
    }
  }
  return total / count;
}

double expectedValueGiven(int a, int b)
{
  double total = 0;
  int count = 0;
  for (int c = 0; c < 6; ++c)
  {
    for (int d = 0; d < 6; ++d)
    {
      for (int e = 0; e < 6; ++e)
      {
        ++count;
        total += values[a][b][c][d][e];
      }
    }
  }
  return total / count;
}

double expectedValueGiven(int a, int b, int c)
{
  double total = 0;
  int count = 0;
  for (int d = 0; d < 6; ++d)
  {
    for (int e = 0; e < 6; ++e)
    {
      ++count;
      total += values[a][b][c][d][e];
    }
  }
  return total / count;
}

double expectedValueGiven(int a, int b, int c, int d)
{
  double total = 0;
  int count = 0;
  for (int e = 0; e < 6; ++e)
  {
    ++count;
    total += values[a][b][c][d][e];
  }
  return total / count;
}

std::pair<double, std::string> get_best(std::array<int, 5> dice, bool verbose = false)
{
  std::multimap<double, std::string> actions;
  actions.emplace(values[dice[0]][dice[1]][dice[2]][dice[3]][dice[4]] / 10, "lock nothing");

  for (int i = 0; i < 5; ++i)
  {
    std::stringstream ss;
    ss << "lock " << i + 1 << " (value=" << dice[i] + 1 << ")";
    actions.emplace(expectedValueGiven(dice[i]) / 15, ss.str());
  }
  for (int i = 0; i < 5; ++i)
  {
    for (int j = i + 1; j < 5; ++j)
    {
      std::stringstream ss;
      ss << "lock " << i + 1 << j + 1 << " (values=" << dice[i] + 1 << dice[j] + 1 << ")";
      actions.emplace(expectedValueGiven(dice[i], dice[j]) / 20, ss.str());
    }
  }
  for (int i = 0; i < 5; ++i)
  {
    for (int j = i + 1; j < 5; ++j)
    {
      for (int k = j + 1; k < 5; ++k)
      {
        std::stringstream ss;
        ss << "lock " << i + 1 << j + 1 << k + 1 << " (values=" << dice[i] + 1 << dice[j] + 1 << dice[k] + 1 << ")";
        actions.emplace(expectedValueGiven(dice[i], dice[j], dice[k]) / 30, ss.str());
      }
    }
  }
  for (int i = 0; i < 5; ++i)
  {
    for (int j = i + 1; j < 5; ++j)
    {
      for (int k = j + 1; k < 5; ++k)
      {
        for (int l = k + 1; l < 5; ++l)
        {
          std::stringstream ss;
          ss << "lock " << i + 1 << j + 1 << k + 1 << l + 1 << " (values=" << dice[i] + 1 << dice[j] + 1 << dice[k] + 1 << dice[l] + 1 << ")";
          actions.emplace(expectedValueGiven(dice[i], dice[j], dice[k], dice[l]) / 50, ss.str());
        }
      }
    }
  }

  if (verbose)
  {
    for (const auto [value, action] : actions)
    {
      std::cout << action << ": " << value << std::endl;
    }
  }

  auto last = actions.end();
  last--;
  return *last;
}

double average_best()
{
  double total = 0;
  int count = 0;
  for (int a = 0; a < 6; ++a)
  {
    for (int b = 0; b < 6; ++b)
    {
      for (int c = 0; c < 6; ++c)
      {
        for (int d = 0; d < 6; ++d)
        {
          for (int e = 0; e < 6; ++e)
          {
            ++count;
            total += get_best({a, b, c, d, e}).first;
            // std::cout << "best of " << a << b << c << d << e << ": " << get_best({a,b,c,d,e}) << std::endl;
          }
        }
      }
    }
  }
  return total / count;
}

std::array<int, 5> input_dices()
{
  int dices;
  std::cin >> dices;

  std::array<int, 5> dice;
  dice[4] = dices % 10;
  dices = (dices - dices % 10) / 10;
  dice[3] = dices % 10;
  dices = (dices - dices % 10) / 10;
  dice[2] = dices % 10;
  dices = (dices - dices % 10) / 10;
  dice[1] = dices % 10;
  dices = (dices - dices % 10) / 10;
  dice[0] = dices % 10;

  std::cout << "got:";
  for (int &v : dice)
  {
    if (v <= 0 || v > 6)
    {
      throw std::invalid_argument{"invalid dice value"};
    }
    v--; // make it 0-based
    std::cout << " " << v;
  }
  std::cout << std::endl;

  return dice;
}

void calculate_values(int multipler, int all_bingo_counter, int all_bingo_total)
{
  for (int a = 0; a < 6; ++a)
  {
    for (int b = 0; b < 6; ++b)
    {
      for (int c = 0; c < 6; ++c)
      {
        for (int d = 0; d < 6; ++d)
        {
          for (int e = 0; e < 6; ++e)
          {
            values[a][b][c][d][e] = calculate_value({a, b, c, d, e}, all_bingo_counter, all_bingo_total) * multipler;
            // std::cout << "value of " << a << b << c << d << e << ": " <<
            // values[a][b][c][d][e] << std::endl;
          }
        }
      }
    }
  }
}

int main(void)
{
  std::cout << "enter multipler: ";
  int multipler = 0;
  std::cin >> multipler;

  std::cout << "enter current all bingo counter: ";
  int all_bingo_counter = 0;
  std::cin >> all_bingo_counter;

  std::cout << "enter all_bingo_total: ";
  int all_bingo_total = 0;
  std::cin >> all_bingo_total;

  while (true)
  {
    calculate_values(multipler, all_bingo_counter, all_bingo_total);

    std::cout << "average best: " << average_best() << std::endl;

    std::cout << "enter dices: ";
    auto dices = input_dices();

    const auto best = get_best(dices, /*verbose=*/true);
    std::cout << "best: " << best.second << ". value per cost: " << best.first << std::endl;

    std::cout << "enter result dices: ";
    dices = input_dices();
    const int v = (int)values[dices[0]][dices[1]][dices[2]][dices[3]][dices[4]] / multipler;
    std::cout << "value: " << v;
    all_bingo_counter += v;
    std::cout << " now counter: " << all_bingo_counter << std::endl;
  }
  return 0;
}
