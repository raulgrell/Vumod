foreign class Vec3 {
  construct new(x, y, z) {}

  foreign norm()
  foreign dot( rhs )
  foreign cross( rhs )

  foreign x
  foreign x=( rhs )
  foreign y
  foreign y=( rhs )
  foreign z
  foreign z=( rhs )
}