use std::ffi::c_void;
use std::ops;

// same data-layout as engine's vec3_t,
pub struct Vector {
  x: f32,
  y: f32,
  z: f32,
}
impl Vector {
  fn new(&self) -> Vector {
    Vector {
      x: 0.0,
      y: 0.0,
      z: 0.0,
    }
  }

  pub fn compare(&self, vector: Vector) -> bool {
    return self.x == vector.x
      && self.y == vector.y
      && self.z == vector.z;
  }

  pub fn subtract(&mut self, vector: Vector) -> &Vector {
    self.x -= vector.x;
    self.y -= vector.y;
    self.z -= vector.z;

    return self;
  }

  pub fn copy_to_array(self: &Vector, array: &mut [f32]) {
    array[0] = self.x;
    array[1] = self.y;
    array[2] = self.z;
  }

  pub fn length_squared(self: &Vector) -> f32 {
    let Vector { x, y, z } = self;

    return x * x
      + y * y
      + z * z;
  }

  pub fn length(self: &Vector) -> f32 {
    return Vector::length_squared(self).sqrt();
  }

  pub fn normalize(self: &Vector) -> Vector {
    let mut length: f32 = self.length();

    if length == 0.0 {
      return Vector {
        x: 0.0,
        y: 0.0,
        z: 1.0 // ???? why z = 1?
      };
    }

    length = 1.0 / length;

    return self * length;
  }

  pub fn make2d(self: &Vector) -> Vector2d {
    let Vector2d { x, y } = self;

    return Vector2d { x: *x, y: *y };
  }
}
trait VectorWithConstructorArgsTrait {
  fn new(vector: Vector) -> Vector;
}
impl VectorWithConstructorArgsTrait for Vector {
  fn new(vector: Vector) -> Vector {
    Vector { ..vector }
  }
}
impl ops::Add<Vector> for Vector {
  type Output = Vector;

  fn add(&mut self, vector: Vector) -> Vector {
    let result = Vector {
      x: self.x + vector.x,
      y: self.y + vector.y,
      z: self.z + vector.z
    };

    return result;
  }
}
impl ops::AddAssign<Vector> for Vector {
  fn add_assign(&mut self, vector: Vector) {
    self.x += vector.x;
    self.y += vector.y;
    self.z += vector.z;
  }
}
impl ops::Div<Vector> for Vector {
  type Output = Vector;

  fn div(&mut self, value: f32) -> &Vector {
    self.x /= value;
    self.y /= value;
    self.z /= value;

    return self;
  }
}
impl ops::Mul<Vector> for Vector {
  type Output = Vector;

  fn mul(&mut self, value: f32) -> &Vector {
    self.x *= value;
    self.y *= value;
    self.z *= value;

    return self;
  }
}


pub struct Vector2d {
  x: f32,
  y: f32,
}
impl Vector2d {
  pub fn new(x: f32, y: f32) -> Vector2d {
    return Vector2d { x, y };
  }

  pub fn length(self: &Vector2d) -> f32 {
    let Vector2d { x, y } = self;

    return (x * x + y * y).sqrt();
  }

  pub fn normalize(self: &Vector2d) -> Vector2d {
    let mut length: f32 = self.length();

    if length == 0.0 {
      return Vector2d { x: 0.0, y: 0.0 }; // ???? why z = 1?
    }

    length = 1.0 / length;

    return self * length;
  }
}
impl ops::Add<Vector2d> for Vector2d {
  type Output = Vector2d;

  fn add(&mut self, vector: Vector2d) -> Vector2d {
    return Vector2d {
      x: self.x + vector.x,
      y: self.y + vector.y
    };
  }
}
impl ops::AddAssign<Vector2d> for Vector2d {
  fn add_assign(&mut self, vector: Vector2d) {
    self.x += vector.x;
    self.y += vector.y;
  }
}
impl ops::Div<Vector2d> for Vector2d {
  type Output = Vector2d;

  fn div(&mut self, value: f32) -> &Vector2d {
    self.x /= value;
    self.y /= value;

    return self;
  }
}
impl ops::Mul<Vector2d> for Vector2d {
  type Output = Vector2d;

  fn mul(&mut self, value: f32) -> &Vector2d {
    self.x *= value;
    self.y *= value;

    return self;
  }
}

