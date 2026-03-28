

internalf r32 m_ease_in_sine_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - cos_r32((val * PI_R32) / 2.0f);
}

internalf r32 m_ease_out_sine_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return sin_r32((val * PI_R32) / 2.0f);
}

internalf r32 m_ease_in_out_sine_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return -(cos_r32(PI_R32 * val) - 1.0f) / 2.0f;
}

internalf r32 m_ease_in_quad_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return val * val;
}

internalf r32 m_ease_out_quad_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - (1.0f - val) * (1.0f - val);
}

internalf r32 m_ease_in_out_quad_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = 2.0f * val * val;
  } else {
    r32 temp = -2.0f * val + 2.0f;
    val = 1.0f - temp * temp / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_cubic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return val * val * val;
}

internalf r32 m_ease_out_cubic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - (1.0f - val) * (1.0f - val) * (1.0f - val);
}

internalf r32 m_ease_in_out_cubic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = 4.0f * val * val * val;
  } else {
    r32 temp = -2.0f * val + 2.0f;
    val = 1.0f - temp * temp * temp / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_quart_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return val * val * val * val;
}

internalf r32 m_ease_out_quart_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - (1.0f - val) * (1.0f - val) * (1.0f - val) * (1.0f - val);
}

internalf r32 m_ease_in_out_quart_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = 8.0f * val * val * val * val;
  } else {
    r32 temp = -2.0f * val + 2.0f;
    val = 1.0f - temp * temp * temp * temp / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_quint_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return val * val * val * val * val;
}

internalf r32 m_ease_out_quint_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - (1.0f - val) * (1.0f - val) * (1.0f - val) * (1.0f - val) * (1.0f - val);
}

internalf r32 m_ease_in_out_quint_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = 16.0f * val * val * val * val * val;
  } else {
    r32 temp = -2.0f * val + 2.0f;
    val = 1.0f - temp * temp * temp * temp * temp / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_expo_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val != 0.0f) {
    val = pow_r32(2.0f, 10.0f * val - 10.0f);
  }
  
  return val;
}

internalf r32 m_ease_out_expo_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val != 1.0f) {
    val = 1.0f - pow_r32(2.0f, -10.0f * val);
  }
  
  return val;
}

internalf r32 m_ease_in_out_expo_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val != 0.0f && val != 1.0f) {
    if(val < 0.5f) {
      val = pow_r32(2.0f, 20.0f * val - 10.0f) / 2.0f;
    } else {
      val = 2.0f - pow_r32(2.0f, -20.0f * val + 10.0f) / 2.0f;
    }
  }
  
  return val;
}

internalf r32 m_ease_in_circ_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - sqrt_r32(1.0f - val * val);
}

internalf r32 m_ease_out_circ_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return sqrt_r32(1.0f - (1.0f - val) * (1.0f - val));
}

internalf r32 m_ease_in_out_circ_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = (1.0f - sqrt_r32(1.0f - (2.0f * val) * (2.0f * val))) / 2.0f;
  } else {
    val = (1.0f + sqrt_r32(1.0f - (-2.0f * val + 2.0f) * (-2.0f * val + 2.0f))) / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_back_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return (2.70158f * val * val * val) - (1.70158f * val * val);
}

internalf r32 m_ease_out_back_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f + (2.70158f * (val - 1.0f) * (val - 1.0f) * (val - 1.0f)) + (1.70158f * (val - 1.0f) * (val - 1.0f));
}

internalf r32 m_ease_in_out_back_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    r32 temp = (2.0f * val);
    val = (temp * temp) * (3.5949095f * temp - 3.5949095f) / 2.0f;
  } else {
    r32 temp = (2.0f * (val - 2.0f));
    val = (temp * temp) * ((3.5949095f * temp + 3.5949095f) + 2.0f) / 2.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_elastic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val != 0.0f && val != 1.0f) {
    val = -1.0f * pow_r32(2.0f, 10.0f * val - 10.0f) * sin_r32((val * 10.0f - 10.75f) * (2.0f * PI_R32 / 3.0f));
  }
  
  return val;
}

internalf r32 m_ease_out_elastic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val != 0.0f && val != 1.0f) {
    val = pow_r32(2.0f, -10.0f * val) * sin_r32((val * 10.0f + 0.75f) * (2.0f * PI_R32 / 3.0f)) + 1.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_out_elastic_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = -1.0f * pow_r32(2.0f, 20.0f * val - 10.0f) * sin_r32((20.0f * val - 11.125f) * (2.0f * PI_R32 / 4.5f)) / 2.0f;
  } else {
    val = pow_r32(2.0f, -20.0f * val + 10.0f) * sin_r32((20.0f * val - 11.125f) * (2.0f * PI_R32 / 4.5f)) / 2.0f + 1.0f;
  }
  
  return val;
}

internalf r32 m_ease_in_bounce_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  return 1.0f - m_ease_out_bounce_r32(1.0f - val);
}

internalf r32 m_ease_out_bounce_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  r32 n = 7.5625f;
  r32 d = 2.75f;
  
  if(val < (1.0f / d)) {
    val = n * val * val;
  } else if(val < (2.0f / d)) {
    r32 temp = val - 1.5f;
    val = n * (temp / d) * val + 0.75f;
  } else if(val < (2.5f / d)) {
    r32 temp = val - 2.25f;
    val = n * (temp / d) * val + 0.9375f;
  } else {
    r32 temp = val - 2.625f;
    val = n * (temp / d) * val + 0.984375f;
  }
  
  return val;
}

internalf r32 m_ease_in_out_bounce_r32(r32 val) {
  val = clamp(0.0f, val, 1.0f);
  
  if(val < 0.5f) {
    val = 1.0f - m_ease_in_bounce_r32(1.0f - 2.0f * val) / 2.0f;
  } else {
    val = 1.0f + m_ease_out_bounce_r32(2.0f * val - 1.0f) / 2.0f;
  }
  
  return val;
}


internalf r64 m_ease_in_sine_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - cos_r64((val * PI_R64) / 2.0);
}

internalf r64 m_ease_out_sine_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return sin_r64((val * PI_R64) / 2.0);
}

internalf r64 m_ease_in_out_sine_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return -(cos_r64(PI_R64 * val) - 1.0f) / 2.0;
}

internalf r64 m_ease_in_quad_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return val * val;
}

internalf r64 m_ease_out_quad_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - (1.0 - val) * (1.0 - val);
}

internalf r64 m_ease_in_out_quad_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = 2.0 * val * val;
  } else {
    r64 temp = -2.0 * val + 2.0;
    val = 1.0 - temp * temp / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_cubic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return val * val * val;
}

internalf r64 m_ease_out_cubic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - (1.0 - val) * (1.0 - val) * (1.0 - val);
}

internalf r64 m_ease_in_out_cubic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = 4.0 * val * val * val;
  } else {
    r64 temp = -2.0 * val + 2.0;
    val = 1.0 - temp * temp * temp / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_quart_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return val * val * val * val;
}

internalf r64 m_ease_out_quart_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - (1.0 - val) * (1.0 - val) * (1.0 - val) * (1.0 - val);
}

internalf r64 m_ease_in_out_quart_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = 8.0 * val * val * val * val;
  } else {
    r64 temp = -2.0 * val + 2.0;
    val = 1.0 - temp * temp * temp * temp / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_quint_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return val * val * val * val * val;
}

internalf r64 m_ease_out_quint_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - (1.0 - val) * (1.0 - val) * (1.0 - val) * (1.0 - val) * (1.0 - val);
}

internalf r64 m_ease_in_out_quint_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = 16.0 * val * val * val * val * val;
  } else {
    r64 temp = -2.0 * val + 2.0;
    val = 1.0 - temp * temp * temp * temp * temp / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_expo_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val != 0.0) {
    val = pow_r64(2.0, 10.0 * val - 10.0);
  }
  
  return val;
}

internalf r64 m_ease_out_expo_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val != 1.0) {
    val = 1.0 - pow_r64(2.0, -10.0 * val);
  }
  
  return val;
}

internalf r64 m_ease_in_out_expo_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val != 0.0 && val != 1.0) {
    if(val < 0.5) {
      val = pow_r64(2.0, 20.0 * val - 10.0) / 2.0;
    } else {
      val = 2.0 - pow_r64(2.0, -20.0 * val + 10.0) / 2.0;
    }
  }
  
  return val;
}

internalf r64 m_ease_in_circ_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - sqrt_r64(1.0 - val * val);
}

internalf r64 m_ease_out_circ_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return sqrt_r64(1.0 - (1.0 - val) * (1.0 - val));
}

internalf r64 m_ease_in_out_circ_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = (1.0 - sqrt_r64(1.0 - (2.0 * val) * (2.0 * val))) / 2.0;
  } else {
    val = (1.0 + sqrt_r64(1.0 - (-2.0 * val + 2.0) * (-2.0 * val + 2.0))) / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_back_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return (2.70158 * val * val * val) - (1.70158 * val * val);
}

internalf r64 m_ease_out_back_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 + (2.70158 * (val - 1.0) * (val - 1.0) * (val - 1.0)) + (1.70158 * (val - 1.0) * (val - 1.0));
}

internalf r64 m_ease_in_out_back_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    r64 temp = (2.0 * val);
    val = (temp * temp) * (3.5949095 * temp - 3.5949095) / 2.0;
  } else {
    r64 temp = (2.0 * (val - 2.0));
    val = (temp * temp) * ((3.5949095 * temp + 3.5949095) + 2.0) / 2.0;
  }
  
  return val;
}

internalf r64 m_ease_in_elastic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val != 0.0 && val != 1.0) {
    val = -1.0 * pow_r64(2.0, 10.0 * val - 10.0) * sin_r64((val * 10.0 - 10.75) * (2.0 * PI_R64 / 3.0));
  }
  
  return val;
}

internalf r64 m_ease_out_elastic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val != 0.0 && val != 1.0) {
    val = pow_r64(2.0, -10.0 * val) * sin_r64((val * 10.0 + 0.75) * (2.0 * PI_R64 / 3.0)) + 1.0;
  }
  
  return val;
}

internalf r64 m_ease_in_out_elastic_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = -1.0 * pow_r64(2.0, 20.0 * val - 10.0) * sin_r64((20.0 * val - 11.125) * (2.0 * PI_R64 / 4.5)) / 2.0;
  } else {
    val = pow_r64(2.0, -20.0 * val + 10.0) * sin_r64((20.0 * val - 11.125) * (2.0 * PI_R64 / 4.5)) / 2.0 + 1.0;
  }
  
  return val;
}

internalf r64 m_ease_in_bounce_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  return 1.0 - m_ease_out_bounce_r64(1.0 - val);
}

internalf r64 m_ease_out_bounce_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  r64 n = 7.5625;
  r64 d = 2.75;
  
  if(val < (1.0 / d)) {
    val = n * val * val;
  } else if(val < (2.0 / d)) {
    r64 temp = val - 1.5;
    val = n * (temp / d) * val + 0.75;
  } else if(val < (2.5 / d)) {
    r64 temp = val - 2.25;
    val = n * (temp / d) * val + 0.9375;
  } else {
    r64 temp = val - 2.625;
    val = n * (temp / d) * val + 0.984375;
  }
  
  return val;
}

internalf r64 m_ease_in_out_bounce_r64(r64 val) {
  val = clamp(0.0, val, 1.0);
  
  if(val < 0.5) {
    val = 1.0 - m_ease_in_bounce_r64(1.0 - 2.0 * val) / 2.0;
  } else {
    val = 1.0 + m_ease_out_bounce_r64(2.0 * val - 1.0) / 2.0;
  }
  
  return val;
}
