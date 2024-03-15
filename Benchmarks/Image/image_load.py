import torch
import torchvision.transforms as transforms
import io
from PIL import Image

def image_processing():
    image_size = torch.randint(128, 512, (1,)).item()
    
    # Generate a random image tensor (assuming grayscale for simplicity)
    img = torch.randint(0, 256, size=(1, image_size, image_size), dtype=torch.float32)

    # Normalize image to [0, 1] range (common practice in PyTorch)
    normalize = transforms.Normalize(mean=[0.5], std=[0.5])
    img = normalize(img)

    # Perform various image processing operations
    processed_images = {}

    # Resizing
    resize = transforms.Resize((320, 320))
    resized_img = resize(img.clone())  # Clone to avoid modifying original tensor
    processed_images["resized"] = resized_img

    # Blurring (Gaussian blur)
    gaussian_blur = transforms.GaussianBlur(kernel_size=5, sigma=(0.5, 0.5))
    blurred_img = gaussian_blur(img.clone())
    processed_images["blurred"] = blurred_img

    # Contrast adjustment (increase contrast using sigmoid function)
    def sigmoid_contrast(x):
        return torch.sigmoid(1.5 * (x - 0.5))
    contrast_high_img = sigmoid_contrast(img.clone())
    processed_images["contrast_high"] = contrast_high_img

    # Resampling (area interpolation)
    resample = transforms.Resize((256, 256))
    resampled_img = resample(img.clone())
    processed_images["resampled"] = resampled_img

    # Image compression
    def compress_image(image_tensor, quality=10):
        img_pil = transforms.ToPILImage()(image_tensor.cpu().detach().squeeze())
        img_byte_arr = io.BytesIO()
        img_pil.save(img_byte_arr, format='JPEG', quality=quality)
        img_byte_arr = img_byte_arr.getvalue()
        img_reconstructed = Image.open(io.BytesIO(img_byte_arr))
        img_tensor = transforms.ToTensor()(img_reconstructed)
        return img_tensor

    compressed_img = compress_image(img, quality=10)
    processed_images["compressed"] = compressed_img

    return processed_images

