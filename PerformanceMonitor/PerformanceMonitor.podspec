Pod::Spec.new do |s|
  s.name             = 'PerformanceMonitor'
  s.version          = '0.0.1'
  s.summary          = 'PerformanceMonitor'

  s.description      = <<-DESC
                       PerformanceMonitor
                       DESC

  s.homepage         = 'https://code.fineres.com/projects/MO/repos/atom-graphics/browse/PerformanceMonitor'
  s.license          = 'MIT'
  s.author           = { 'neo' => 'neo@fanruan.com' }
  s.source           = { :git => 'ssh://git@code.fineres.com:7999/mo/atom-graphics.git', :tag => s.version }

  s.ios.deployment_target = '8.0'

  s.source_files = "ios/PerformanceMonitor/**/*.{h,m,mm}"

end
