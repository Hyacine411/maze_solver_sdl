from PIL import Image
import os

os.makedirs("assets", exist_ok=True)

# 老鼠图像：灰色
mouse = Image.new("RGB", (16, 16), (100, 100, 100))
mouse.save("assets/mouse.bmp")

# 奶酪图像：金黄色
cheese = Image.new("RGB", (16, 16), (255, 215, 0))
cheese.save("assets/cheese.bmp")

print("✅ BMP 图像已生成：assets/mouse.bmp 和 assets/cheese.bmp")
