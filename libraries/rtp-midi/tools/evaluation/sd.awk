// {
  n++
  total += $1
  sqTotal += $1^2
}

END {
  print sqrt ( sqTotal / n - ( total / n )^2 )
}
