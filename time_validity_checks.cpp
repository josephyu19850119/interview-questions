// 检查一个时间-日期点是否有效，如一分钟不能有第61秒，一个月不能有第32天
// 这个问题不包含任何算法和领域知识，但如果能用代码呈现一个人尽皆知的生活常识，则可以体现候选人代码的可读性和简洁性
bool time_validity_checks(int year, int month, int day, int hour, int minute,
                          int second) {

  // 约定参数year为正时表示公元后，为负时表示公元前，因为没有公元零年，所以需检查其不为零
  // 这是个容易忽略的知识点，虽然这不影响候选人的编程水平
  // 但如果能考虑到这一点并恰当的通过代码实现，则能够体现出候选人的严谨以及对有符号整型的合理运用
  if (year == 0) {
    return false;
  }

  // 因为month既需要被判断有效性，也是判断day有效性的前置条件，而且总共只有12个，所以适合用switch-case语句来合并month与day的判定
  switch (month) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    if (day > 31) {
      return false;
    }
    break;
  case 4:
  case 6:
  case 9:
  case 11:
    if (day > 30) {
      return false;
    }
    break;
  case 2:
    // 闰年的逻辑判断可以有不同的实现，这里的和后面作为对比的代码都是正确的
    // 重要的是用括号使逻辑组合更清晰，而不要完全依赖&&和||的优先级
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
      if (day > 29) {
        return false;
      }
    } else {
      if (day > 28) {
        return false;
      }
    }
    break;
  default:
    return false;
  }

  if (day < 1) {
    return false;
  }

  if (hour < 0 || hour > 23) {
    return false;
  }

  if (minute < 0 || minute > 59) {
    return false;
  }

  if (second < 0 || hour > 59) {
    return false;
  }

  return true;
}

// 以下是候选人一个典型的的实现方式
bool time_validity_checks(int year, int month, int day, int hour, int minute,
                          int second) {

  if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 ||
      second > 59) {
    return false;
  }
  // 没有考虑公元前后和零年的问题不是主要的
  // 一个明显可以改进的地方，就是在大小月的每个分支下，都判断了一遍day < 1
  // 使代码不够精简
  if (month < 1 || month > 12) {
    return false;
  } else if (month == 1 || month == 3 || month == 5 || month == 7 ||
             month == 8 || month == 10 || month == 12) {
    if (day < 1 || day > 31) {
      return false;
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    if (day < 1 || day > 30) {
      return false;
    }
  } else {
    if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) {
      if (day < 1 || day > 29) {
        return false;
      }
    } else {
      if (day < 1 || day > 28) {
        return false;
      }
    }
  }

  return true;
}